//
//  LSystemRulesEngine.cpp
//  burningbush
//
//  Created by Michael Dewberry on 1/29/15.
//
//

#include "LSystemRulesEngine.h"


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

void LSystemRulesEngine::getMatchingRules(const RuleString& current, const RuleString::iterator& currentPos, ProductionRuleGroup& matched) const {
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
            contextFreeMatches.push_back(rule);
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

