//
//  LSystem.cpp
//  burningbush
//
//  Created by Dewb on 12/26/14.
//
//

#include "LSystem.h"
#include <tr1/random>
#include <iostream>


std::ostream& operator<<(std::ostream& os, const RuleToken& token) {
    os << token.symbol;
    if (!token.subscript.empty()) {
        os << "_" << token.subscript;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const RuleString& str) {
    for (auto& token : str) {
        os << token;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const ProductionRule& rule) {
    if (!rule.leftContext.empty()) {
        os << rule.leftContext;
        os << " < ";
    }
    os << rule.predecessor;
    if (!rule.rightContext.empty()) {
        os << " > ";
        os << rule.rightContext;
    }
    os << " -> ";
    os << rule.successor;
    return os;
}

bool RuleToken::operator<(const RuleToken& rhs) const {
    return this->symbol < rhs.symbol;
}

bool RuleToken::operator>(const RuleToken& rhs) const {
    return this->symbol > rhs.symbol;
}

bool RuleToken::operator==(const RuleToken& rhs) const {
    return this->symbol == rhs.symbol;
}

bool RuleToken::operator!=(const RuleToken& rhs) const {
    return this->symbol != rhs.symbol;
}

bool RuleToken::operator==(const string& rhs) const {
    return this->symbol == rhs;
}

bool RuleToken::operator==(const char& rhs) const {
    return this->symbol.size() == 1 && this->symbol[0] == rhs;
}

RuleToken parseRuleToken(const string& str, int pos = 0) {
    RuleToken token(str[pos]);
    if (pos + 2 < str.size() && str[pos + 1] == '_') {
        token.subscript = str[pos + 2];
    }
    return token;
}

string to_string(const RuleString& rs) {
    ostringstream ss;
    for (const RuleToken& t : rs) {
        ss << t;
    }
    return ss.str();
}

RuleString parseRuleString(const string& str) {
    RuleString r;
    for (int i = 0; i < str.size(); i++) {
        RuleToken token = parseRuleToken(str, i);
        r.push_back(token);
    }
    return r;
}

LSystem::LSystem() {
    seed = abs(rand());
}

void LSystem::setAxiom(const RuleString& axiomString) {
    axiom = axiomString;
}

void LSystem::setAxiom(const string& axiomString) {
    axiom = parseRuleString(axiomString);
}

void LSystem::ignoreForContext(const RuleString& ignoreString) {
    ignoreContext = ignoreString;
}

void LSystem::ignoreForContext(const string& ignoreString) {
    ignoreContext = parseRuleString(ignoreString);
}

ProductionRule& LSystem::addRule(const RuleToken& predeccessor, const RuleString& successor) {
    ProductionRule rule(predeccessor, successor);
    
    if (rules.find(predeccessor) == rules.end()) {
        rules.insert(pair<RuleToken, ProductionRuleGroup>(predeccessor, ProductionRuleGroup()));
    }
    rules[predeccessor].push_back(rule);
    return rules[predeccessor].back();
}

ProductionRule& LSystem::addRule(const string& predecessor, const string& successor) {
    return addRule(parseRuleToken(predecessor), parseRuleString(successor));
}

ProductionRule& LSystem::addRule(const char predecessor, const string& successor) {
    return addRule(RuleToken(predecessor), parseRuleString(successor));
}

ProductionRule& LSystem::addRule(const RuleString& leftContext, const RuleToken& predecessor, const RuleString& rightContext, const RuleString& successor) {
    
    ProductionRule& rule = addRule(predecessor, successor);
    rule.leftContext = leftContext;
    rule.rightContext = rightContext;
    return rule;
}

ProductionRule& LSystem::addRule(const string& leftContext, const string& predecessor, const string& rightContext, const string& successor) {
    return addRule(parseRuleString(leftContext), parseRuleToken(predecessor),
                   parseRuleString(rightContext), parseRuleString(successor));
}

ProductionRule& LSystem::addRule(const string& leftContext, const char predecessor, const string& rightContext, const string& successor) {
    return addRule(parseRuleString(leftContext), RuleToken(predecessor),
                   parseRuleString(rightContext), parseRuleString(successor));
}


void LSystem::reset() {
    rules.clear();
    properties.clear();
    axiom.clear();
}

void LSystem::reseed(unsigned newSeed) {
    seed = newSeed;
}

void LSystem::reseed() {
    reseed(abs(rand()));
}

bool LSystem::isStochastic() const {
    for (auto& rule : rules) {
        if (rule.second.isStochastic()) {
            return true;
        }
    }
    return false;
}

template <typename Iter>
bool LSystem::contextMatches(const Iter& contextBegin, const Iter& contextEnd,
                             const Iter& stringBegin, const Iter& stringEnd,
                             bool reversed, bool followBranches) const {
    auto currentPos = stringBegin;
    auto contextPos = contextBegin;
    char startBranch = '[';
    char endBranch = ']';
    if (reversed) {
        swap(startBranch, endBranch);
    }
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
                return
                    contextMatches(contextPos, contextEnd, insideStart, insideEnd, reversed, followBranches) ||
                    contextMatches(contextPos, contextEnd, branchEnd, stringEnd, reversed, followBranches);
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
        }
    }
    if (contextPos == contextEnd) {
        return true;
    } else {
        return false;
    }
}

void LSystem::getMatchingRules(const RuleString& current, const RuleString::iterator& currentPos, ProductionRuleGroup& matched) {
    if (rules.find(*currentPos) == rules.end()) {
        return;
    }

    auto nextPos = currentPos;
    nextPos++;
    
    for (auto& rule : rules[*currentPos]) {
        if ((rule.leftContext.empty() ||
                contextMatches<RuleString::const_reverse_iterator>(
                    rule.leftContext.rbegin(), rule.leftContext.rend(),
                    reverse_iterator<RuleString::iterator>(currentPos), current.rend(),
                    true, false
                )) &&
            (rule.rightContext.empty() ||
                contextMatches<RuleString::const_iterator>(
                    rule.rightContext.begin(), rule.rightContext.end(),
                    nextPos, current.end(),
                    false, true
                ))) {
             matched.push_back(rule);
        }
    }
}

typedef pair<RuleString::iterator, RuleString> Replacement;
typedef vector<Replacement> Replacements;

RuleString LSystem::generate(int iterations, bool logging) {
    RuleString current = axiom;
    
    tr1::ranlux_base_01 generator(seed);
    tr1::uniform_real<float> distribution(0.0, 1.0);

    ProductionRuleGroup matchedRules;
    Replacements replacements;
   
    while (iterations--) {
        replacements.clear();
        auto currentPos = current.begin();
        while(currentPos != current.end()) {
            matchedRules.clear();
            getMatchingRules(current, currentPos, matchedRules);
            if (matchedRules.size()) {
                if (!matchedRules.isStochastic()) {
                    // Basic case: just one matching rule
                    if (logging) {
                        cout << "Executing: " << matchedRules[0] << "\n";
                    }
                    replacements.push_back(Replacement(currentPos, matchedRules[0].successor));
                } else {
                    // Stochastic case: multiple rules
                    float totalProbability = 0;
                    for (auto& rule : matchedRules) {
                        totalProbability += rule.probability;
                    }
                    float d = distribution(generator);
                    float p = d * totalProbability;
                    float s = 0;
                    for (auto& rule : matchedRules) {
                        s += rule.probability;
                        if (s > p) {
                            if (logging) {
                                cout << "Executing: " << rule << "\n";
                            }
                            replacements.push_back(Replacement(currentPos, rule.successor));
                            break;
                        }
                    }
                }
            }
            ++currentPos;
        }
        
        for (auto& repl : replacements) {
            current.insert(current.erase(repl.first), repl.second.begin(), repl.second.end());
        }
        
        if (logging) {
            cout << current << "\n";
        }
    }
    return current;
}

float LSystem::getProperty(string name) const {
    auto iter = properties.find(name);
    if (iter != properties.end()) {
        return iter->second;
    } else {
        return 0;
    }
}

void LSystem::setProperty(string name, float value) {
    properties[name] = value;
}

bool LSystem::hasProperty(string name) const {
    return properties.find(name) != properties.end();
}