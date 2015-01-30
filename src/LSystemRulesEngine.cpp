//
//  LSystemRulesEngine.cpp
//  burningbush
//
//  Created by Michael Dewberry on 1/29/15.
//
//

#include "LSystemRulesEngine.h"
#include "../lib/exprtk/exprtk.hpp"
#include <iostream>

typedef exprtk::symbol_table<double>    symbol_table_t;
typedef exprtk::expression<double>        expression_t;
typedef exprtk::parser<double>                parser_t;
typedef typename parser_t::dependent_entity_collector::symbol_t symbol_t;


float evaluateExpression(const RuleToken& predecessor, const RuleToken& tokenMatch, const string& exprString) {
    parser_t parser;
    symbol_table_t symbol_table;
    
    expression_t expression;
    expression.register_symbol_table(symbol_table);
    
    parser.dec().collect_variables() = true;
    parser.enable_unknown_symbol_resolver();
    
    if (!parser.compile(exprString, expression)) {
        cout << "Failed to compile expression: " << exprString << ", error: " << parser.error() << "\n";
    }
    
    std::deque<symbol_t> symbol_list;
    parser.dec().symbols(symbol_list);
    
    for (std::size_t i = 0; i < symbol_list.size(); ++i) {
        symbol_t& symbol = symbol_list[i];
        
        switch (symbol.second) {
            case parser_t::e_st_variable:
                {
                    bool found = false;
                    for (int i = 0; i < predecessor.parameters.size(); i++) {
                        if (predecessor.parameters[i] == symbol.first) {
                            float x;
                            stringstream ss(tokenMatch.parameters[i]);
                            ss >> x;
                            if (ss.fail()) {
                                cout << "ERROR: Argument " << tokenMatch.parameters[i] << " is non-numeric!\n";
                                return false;
                            }
                            
                            //cout << "Assigning " << symbol.first << " to " << x << "\n";
                            symbol_table.variable_ref(symbol.first) = x;
                            found = true;
                        }
                    }
                    if (!found) {
                        cout << "ERROR: Unknown symbol " << symbol.first << " in expression " << exprString << "\n";
                        return false;
                    }
                }
                break;
            default:
                break;
        }
    }
    
    return expression.value();
}

bool conditionMatches(const RuleToken& predecessor, const RuleToken& tokenMatch, const string& condition) {
    if (condition.empty()) {
        return true;
    }
    
    return evaluateExpression(predecessor, tokenMatch, condition) != 0.0;
}

RuleString LSystemRulesEngine::evaluateSuccessor(const RuleToken& predecessor, const RuleToken& tokenMatch, const RuleString& successor) {
    if (!predecessor.isParametric()) {
        return successor;
    }
    RuleString result = successor;
    for (auto& token : result) {
        for (int i = 0; i < token.parameters.size(); i++) {
            float value = evaluateExpression(predecessor, tokenMatch, token.parameters[i]);
            token.parameters[i] = to_string(value);
        }
    }
    return result;
}


template <typename Iter>
bool contextMatches(const RuleString& ignoreContext, const Iter& contextBegin, const Iter& contextEnd,
                             const Iter& stringBegin, const Iter& stringEnd,
                             bool reversed, bool followBranches, int* pTrunkLength = NULL) {
    auto currentPos = stringBegin;
    auto contextPos = contextBegin;
    char startBranch = '[';
    char endBranch = ']';
    if (reversed) {
        swap(startBranch, endBranch);
    }
    int trunkLength = 0;
    
    while (currentPos != stringEnd && contextPos != contextEnd) {
        if(find(ignoreContext.begin(), ignoreContext.end(), *currentPos) != ignoreContext.end()) {
            // skip tokens in ignore string
            ++currentPos;
        } else if (*currentPos == endBranch) {
            if (followBranches) {
                // branch ended without a context match
                return false;
            } else {
                // continue on parent
                ++currentPos;
            }
        } else if (*currentPos == startBranch) {
            // starting branch definition, find the end
            int closeBracketsRequired = 1;
            auto branchEnd = currentPos;
            ++branchEnd;
            while (closeBracketsRequired > 0) {
                if (*branchEnd == startBranch) {
                    closeBracketsRequired++;
                } else if (*branchEnd == endBranch) {
                    closeBracketsRequired--;
                }
                ++branchEnd;
            }
            if (followBranches) {
                // check for a match on both branches
                auto insideStart = ++currentPos;
                auto insideEnd = branchEnd;
                insideEnd--;
                int moreTrunkLength = 0;
                // Prefer staying on this trunk/branch
                if (contextMatches(ignoreContext, contextPos, contextEnd, branchEnd, stringEnd, reversed, followBranches, &moreTrunkLength)) {
                    if (pTrunkLength) {
                        *pTrunkLength = trunkLength + moreTrunkLength;
                    }
                    return true;
                } else if (contextMatches(ignoreContext, contextPos, contextEnd, insideStart, insideEnd, reversed, followBranches)) {
                    if (pTrunkLength) {
                        *pTrunkLength = trunkLength;
                    }
                    return true;
                }
            } else {
                // skip ahead to end
                currentPos = branchEnd;
            }
        } else if (*contextPos != *currentPos) {
            // context did not match
            return false;
        } else {
            // context does match so far
            currentPos++;
            contextPos++;
            trunkLength++;
        }
    }
    if (contextPos == contextEnd) {
        if (pTrunkLength) {
            *pTrunkLength = trunkLength;
        }
        return true;
    } else {
        return false;
    }
}

void LSystemRulesEngine::getMatchingRules(const RuleString& current, const RuleString::iterator& currentPos, ProductionRuleGroup& matched) {
    if (system->rules.find(*currentPos) == system->rules.end()) {
        return;
    }
    
    auto nextPos = currentPos;
    nextPos++;
    
    // If a context-free and a contextual rule match, prefer the contextual;
    // if multiple contextual rules match, prefer the longer one;
    // if multiple contextual rules of the same length match, prefer the one on the axial branch (trunk)
    ProductionRuleGroup contextFreeMatches;
    ProductionRuleGroup contextualMatches;
    int contextMatchLength = 0;
    int trunkMatchLength = 0;
    
    for (auto& rule : system->rules[*currentPos]) {
        if (rule.leftContext.empty() && rule.rightContext.empty()) {
            if (!rule.isParametric() || conditionMatches(rule.predecessor, *currentPos, rule.parametricCondition)) {
                contextFreeMatches.push_back(rule);
            }
        } else {
            if (!rule.leftContext.empty() &&
                !contextMatches<RuleString::const_reverse_iterator>(
                    system->ignoreContext,
                    rule.leftContext.rbegin(), rule.leftContext.rend(),
                    reverse_iterator<RuleString::iterator>(currentPos), current.rend(),
                    true, false))
            {
                continue;
            }
            int rightTrunkLength = 0;
            if (!rule.rightContext.empty() &&
                !contextMatches<RuleString::const_iterator>(
                    system->ignoreContext,
                    rule.rightContext.begin(), rule.rightContext.end(),
                    nextPos, current.end(),
                    false, true, &rightTrunkLength))
            {
                continue;
            }
            
            int contextLength = rule.leftContext.size() + rule.rightContext.size();
            int trunkLength = rule.leftContext.size() + rightTrunkLength;
            
            if (contextLength > contextMatchLength) {
                contextualMatches.clear();
                contextualMatches.push_back(rule);
                contextMatchLength = contextLength;
                trunkMatchLength = trunkLength;
            } else if (contextLength == contextMatchLength) {
                if (trunkLength > trunkMatchLength) {
                    contextualMatches.clear();
                    contextualMatches.push_back(rule);
                    trunkMatchLength = trunkLength;
                } else if (trunkLength == trunkMatchLength) {
                    contextualMatches.push_back(rule);
                }
            }
        }
    }
    
    if (contextualMatches.size()) {
        matched = contextualMatches;
    } else {
        matched = contextFreeMatches;
    }
}

