//
//  LSystem.cpp
//  burningbush
//
//  Created by Dewb on 12/26/14.
//
//

#include "LSystem.h"
#include "LSystemRulesEngine.h"
#include <tr1/random>
#include <iostream>


LSystemOptions::LSystemOptions()
: useCache(false)
, logging(false)
, steps(-1)
{
}

LSystemOptions LSystemOptions::DefaultOptions;

LSystem::LSystem() {
    seed = abs(rand());
}

void LSystem::setAxiom(const RuleString& axiomString) {
    axiom = axiomString;
}

void LSystem::ignoreForContext(const RuleString& ignoreString) {
    ignoreContext = ignoreString;
}

ProductionRule& LSystem::addRule(const RuleToken& predeccessor, const RuleString& successor) {
    ProductionRule rule(predeccessor, successor);
    
    if (rules.find(predeccessor) == rules.end()) {
        rules.insert(pair<RuleToken, ProductionRuleGroup>(predeccessor, ProductionRuleGroup()));
    }
    
    rules[predeccessor].push_back(rule);
    return rules[predeccessor].back();
}

ProductionRule& LSystem::addRule(const RuleString& leftContext, const RuleToken& predecessor, const RuleString& rightContext, const RuleString& successor) {
    
    ProductionRule& rule = addRule(predecessor, successor);
    rule.leftContext = leftContext;
    rule.rightContext = rightContext;
    return rule;
}

void LSystem::reset() {
    rules.clear();
    properties.clear();
    axiom.clear();
    cache.clear();
    title = "";
}

void LSystem::reseed(unsigned newSeed) {
    seed = newSeed;
    cache.clear();
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

typedef pair<RuleString::iterator, RuleString> Replacement;
typedef vector<Replacement> Replacements;

RuleString LSystem::generate(int iterations, LSystemOptions& options) {
    int currentIteration = 1;
    RuleString current = axiom;

    tr1::ranlux_base_01 generator(seed);
    tr1::uniform_real<float> distribution(0.0, 1.0);

    IndexedProductionRulePtrGroup matchedRules;
    Replacements replacements;
    LSystemRulesEngine engine(this);
    
    if (options.logging) {
        cout << current << "\n";
    }

    if (options.useCache) {
        for (auto iter = cache.begin(); iter != cache.end(); iter++) {
            if (iter->first == iterations) {
                return iter->second;
            } else if (iter->first < iterations) {
                currentIteration = iter->first + 1;
                current = iter->second;
                break;
            }
        }
    }

    while (currentIteration <= iterations) {
        replacements.clear();
        auto currentPos = current.begin();
        while(currentPos != current.end()) {
            matchedRules.clear();
            engine.getMatchingRules(current, currentPos, matchedRules);
            if (matchedRules.size()) {
                if (matchedRules.size() == 1) {
                    // Basic case: just one matching rule
                    auto rule = *(matchedRules[0].second);
                    int ruleIndex = matchedRules[0].first;
                    if (options.logging) {
                        cout << "Executing: " << rule << "\n";
                    }
                    RuleString successor =
                        engine.evaluateSuccessor(ruleIndex, rule.predecessor, *currentPos, rule.successor);
                    replacements.push_back(Replacement(currentPos, successor));
                } else {
                    // Stochastic case: multiple rules
                    float totalProbability = 0;
                    for (auto& item : matchedRules) {
                        totalProbability += item.second->probability;
                    }
                    float d = distribution(generator);
                    float p = d * totalProbability;
                    float s = 0;
                    for (auto iter = matchedRules.rbegin(); iter != matchedRules.rend(); ++iter) {
                        auto rule = *(iter->second);
                        int ruleIndex = iter->first;
                        s += rule.probability;
                        if (s > p) {
                            if (options.logging) {
                                cout << "Executing: " << rule << "\n";
                            }
                            RuleString successor =
                                engine.evaluateSuccessor(ruleIndex, rule.predecessor, *currentPos, rule.successor);
                            replacements.push_back(Replacement(currentPos, successor));
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
        
        if (options.logging) {
            cout << current << "\n";
        }

        if (options.useCache) {
            cache.insert(make_pair<int, RuleString>(currentIteration, current));
        }

        currentIteration++;
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

const vector<string> LSystem::getProperties() const {
    vector<string> retval;
    for (auto const& prop : properties) {
        retval.push_back(prop.first);
    }
    return retval;
}