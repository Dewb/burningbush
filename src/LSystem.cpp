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

LSystem::LSystem() {
    seed = abs(rand());
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


ProductionRule& LSystem::addRule(const RuleToken& predeccessor, const RuleString& successor) {
    ProductionRule rule;
    rule.predecessor = predeccessor;
    rule.successor = successor;
    
    if (rules.find(predeccessor) == rules.end()) {
        rules.insert(pair<RuleToken, ProductionRuleGroup>(predeccessor, ProductionRuleGroup()));
    }
    rules[predeccessor].push_back(rule);
    return rules[predeccessor].back();
}

void LSystem::addRule(const RuleString& leftContext, const RuleToken& predeccessor, const RuleString& rightContext, const RuleString& successor) {
 
    ProductionRule& rule = addRule(predeccessor, successor);
    rule.leftContext = leftContext;
    rule.rightContext = rightContext;
}


void LSystem::reset() {
    rules.clear();
    properties.clear();
    axiom = "";
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


bool LSystem::leftContextMatches(const RuleString& leftContext, const RuleString& current, int position) const {
    int lcl = leftContext.length();
    if (position < lcl) {
        return false;
    }
    int currentPos = position - 1;
    int contextPos = lcl - 1;
    while (currentPos >= 0 && contextPos >= 0) {
        while(currentPos >= 0 && ignoreContext.find(current[currentPos]) != ignoreContext.npos)
            currentPos--;
        /*
        if (current[currentPos] == ']') {
            currentPos = current.rfind('[', currentPos);
            RuleString outsideBranch = current.substr(0, branchStart);
            return leftContextMatches(unmatched, outsideBranch, outsideBranch.size() - 1);
        }
         */
        if (currentPos >= 0) {
            if (leftContext[contextPos] != current[currentPos]) {
                return false;
            }
            currentPos--;
            contextPos--;
        }
    }
    if (contextPos == -1) {
        return true;
    } else {
        return false;
    }
}

bool LSystem::rightContextMatches(const RuleString& rightContext, const RuleString& current, int position) const {
    int rcl = rightContext.length();
    if (position < rcl) {
        return false;
    }
    int currentPos = position + 1;
    int contextPos = 0;
    while (currentPos < current.size() && contextPos < rcl) {
        while(currentPos < current.size() && ignoreContext.find(current[currentPos]) != ignoreContext.npos)
            currentPos++;
        /*
        if (current[currentPos] == '[') {
            RuleString unmatched = rightContext.substr(contextPos);
            int branchEnd = current.find('[', currentPos);
            RuleString insideBranch = current.substr(currentPos + 1, branchEnd - currentPos - 1);
            RuleString outsideBranch = current.substr(branchEnd + 1);
            return
                rightContextMatches(unmatched, insideBranch, 0) ||
                rightContextMatches(unmatched, outsideBranch, 0);
        }
        */
        if (currentPos < current.size()) {
            if (rightContext[contextPos] != current[currentPos]) {
                return false;
            }
            currentPos++;
            contextPos++;
        }
    }
    if (contextPos == rcl) {
        return true;
    } else {
        return false;
    }
}

void LSystem::getMatchingRules(const RuleString& current, int position, ProductionRuleGroup& matched) {
    RuleToken currentToken = current[position];
    if (rules.find(currentToken) == rules.end()) {
        return;
    }

    for (auto& rule : rules[currentToken]) {
        if ((rule.leftContext.empty() || leftContextMatches(rule.leftContext, current, position)) &&
            (rule.rightContext.empty() || rightContextMatches(rule.rightContext, current, position))) {
             matched.push_back(rule);
        }
    }
}

typedef pair<int, RuleString> Replacement;
typedef vector<Replacement> Replacements;

RuleString LSystem::generate(int iterations) {
    RuleString current = axiom;
    
    tr1::ranlux_base_01 generator(seed);
    tr1::uniform_real<float> distribution(0.0, 1.0);

    ProductionRuleGroup matchedRules;
    Replacements replacements;
   
    while (iterations--) {
        size_t position = 0;
        replacements.clear();
        
        while(position < current.length()) {
            matchedRules.clear();
            getMatchingRules(current, position, matchedRules);
            if (matchedRules.size()) {
                if (!matchedRules.isStochastic()) {
                    // Basic case: just one matching rule
                    cout << "Executing: " << matchedRules[0] << "\n";
                    replacements.push_back(Replacement(position, matchedRules[0].successor));
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
                            cout << "Executing: " << rule << "\n";
                            replacements.push_back(Replacement(position, rule.successor));
                            break;
                        }
                    }
                }
            }
            position++;
        }
        
        int shift = 0;
        for (auto& repl : replacements) {
            current.replace(repl.first + shift, 1, repl.second);
            shift += repl.second.length() - 1;
        }
        
        cout << current << "\n";
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