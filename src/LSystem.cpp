//
//  LSystem.cpp
//  burningbush
//
//  Created by Dewb on 12/26/14.
//
//

#include "LSystem.h"
#include <tr1/random>

LSystem::LSystem() {
    seed = abs(rand());
}

void LSystem::addRule(const RuleToken& predeccessor, const RuleString& successor) {
    ProductionRule rule;
    rule.predecessor = predeccessor;
    rule.successor = successor;
    
    if (rules.find(predeccessor) == rules.end()) {
        rules.insert(pair<RuleToken, ProductionRuleGroup>(predeccessor, ProductionRuleGroup()));
    }
    rules[predeccessor].push_back(rule);
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

RuleString LSystem::generate(int iterations) {
    RuleString current = axiom;
    
    tr1::ranlux_base_01 generator(seed);
    tr1::uniform_real<float> distribution(0.0, 1.0);
   
    while (iterations--) {
        size_t position = 0;
        while(position < current.length()) {
            RuleToken currentToken = current[position];
            if (rules.find(currentToken) != rules.end()) {
                auto matchedRules = rules[currentToken];
                // Basic case: just one matching rule
                if (matchedRules.size() == 1) {
                    current.replace(position, 1, matchedRules[0].successor);
                    position += matchedRules[0].successor.length();
                    continue;
                }
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
                        current.replace(position, 1, rule.successor);
                        position += rule.successor.length() - 1;
                        break;
                    }
                }
            }
            position++;
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