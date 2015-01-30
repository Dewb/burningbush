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


std::ostream& operator<<(std::ostream& os, const RuleToken& token) {
    os << token.symbol;
    if (!token.subscript.empty()) {
        os << "_" << token.subscript;
    }
    if (token.parameters.size() > 0) {
        os << "(";
        for (int i = 0; i < token.parameters.size(); i++) {
            os << token.parameters[i];
            if (i != token.parameters.size() - 1) {
                os << ",";
            }
        }
        os << ")";
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

string to_string(const RuleToken& rt) {
    ostringstream ss;
    ss << rt;
    return ss.str();
}

string to_string(const RuleString& rs) {
    ostringstream ss;
    for (const RuleToken& t : rs) {
        ss << t;
    }
    return ss.str();
}

string to_string(const ProductionRule& pr) {
    ostringstream ss;
    ss << pr;
    return ss.str();
}

bool RuleToken::operator<(const RuleToken& rhs) const {
    if (this->symbol == rhs.symbol) {
        if (this->subscript == rhs.subscript) {
            return this->parameters.size() < rhs.parameters.size();
        } else {
            return this->subscript < rhs.subscript;
        }
    } else {
        return this->symbol < rhs.symbol;
    }
}

bool RuleToken::operator>(const RuleToken& rhs) const {
    if (this->symbol == rhs.symbol) {
        if (this->subscript == rhs.subscript) {
            return this->parameters.size() > rhs.parameters.size();
        } else {
            return this->subscript > rhs.subscript;
        }
    } else {
        return this->symbol > rhs.symbol;
    }
}

bool RuleToken::operator==(const RuleToken& rhs) const {
    if (this->symbol == rhs.symbol) {
        if (this->subscript == rhs.subscript) {
            return this->parameters.size() == rhs.parameters.size();
        } else {
            return this->subscript == rhs.subscript;
        }
    } else {
        return false;
    }
}

bool RuleToken::operator!=(const RuleToken& rhs) const {
    if (this->symbol == rhs.symbol) {
        if (this->subscript == rhs.subscript) {
            return this->parameters.size() != rhs.parameters.size();
        } else {
            return this->subscript != rhs.subscript;
        }
    } else {
        return true;
    }
}

bool RuleToken::operator==(const string& rhs) const {
    return this->symbol == rhs;
}

bool RuleToken::operator==(const char& rhs) const {
    return this->symbol.size() == 1 && this->symbol[0] == rhs;
}

RuleToken parseRuleToken(const string& str, int* pStartPosition = NULL) {
    int pos = 0;
    if (pStartPosition) {
        pos = *pStartPosition;
    }
    RuleToken token(str[pos]);
    pos++;
    if (pos + 1 < str.size() && str[pos] == '_') {
        token.subscript = str[pos + 1];
        pos += 2;
    }
    if (pos + 1 < str.size() && str[pos] == '(') {
        int parenLevel = 1;
        pos = pos + 1;
        int startPos = pos;
        while (parenLevel > 0 && pos < str.size()) {
            if (str[pos] == ')') {
                parenLevel--;
                pos++;
            } else if (str[pos] == '(') {
                parenLevel++;
                pos++;
            } else if (str[pos] == ',' && parenLevel == 1) {
                token.parameters.push_back(str.substr(startPos, pos - startPos));
                pos++;
                startPos = pos;
            } else {
                pos++;
            }
        }
        if (parenLevel != 0) {
            cout << "Unmatched parentheses in token!\n";
        } else if (pos < str.size() + 1) {
            token.parameters.push_back(str.substr(startPos, pos - startPos - 1));
        }
    }
    if (pStartPosition) {
        *pStartPosition = pos;
    }
    return token;
}

RuleString parseRuleString(const string& str) {
    RuleString r;
    int i = 0;
    while(i < str.size()) {
        RuleToken token = parseRuleToken(str, &i);
        r.push_back(token);
    }
    return r;
}

bool ProductionRuleGroup::isStochastic() const {
    for (auto& item : *this) {
        if (item.isStochastic()) {
            return true;
        }
    }
    return false;
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

ProductionRule& LSystem::addRule(const RuleToken& predeccessor, const RuleString& successor, float prob) {
    ProductionRule rule(predeccessor, successor);
    
    if (rules.find(predeccessor) == rules.end()) {
        rules.insert(pair<RuleToken, ProductionRuleGroup>(predeccessor, ProductionRuleGroup()));
    }
    rule.probability = prob;
    
    rules[predeccessor].push_back(rule);
    return rules[predeccessor].back();
}

ProductionRule& LSystem::addRule(const string& predecessor, const string& successor, float prob) {
    return addRule(parseRuleToken(predecessor), parseRuleString(successor), prob);
}

ProductionRule& LSystem::addRule(const char predecessor, const string& successor, float prob) {
    return addRule(RuleToken(predecessor), parseRuleString(successor), prob);
}

ProductionRule& LSystem::addRule(const RuleString& leftContext, const RuleToken& predecessor, const RuleString& rightContext, const RuleString& successor, float prob) {
    
    ProductionRule& rule = addRule(predecessor, successor, prob);
    rule.leftContext = leftContext;
    rule.rightContext = rightContext;
    return rule;
}

ProductionRule& LSystem::addRule(const string& leftContext, const string& predecessor, const string& rightContext, const string& successor, float prob) {
    return addRule(parseRuleString(leftContext), parseRuleToken(predecessor),
                   parseRuleString(rightContext), parseRuleString(successor), prob);
}

ProductionRule& LSystem::addRule(const string& leftContext, const char predecessor, const string& rightContext, const string& successor, float prob) {
    return addRule(parseRuleString(leftContext), RuleToken(predecessor),
                   parseRuleString(rightContext), parseRuleString(successor), prob);
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

typedef pair<RuleString::iterator, RuleString> Replacement;
typedef vector<Replacement> Replacements;

RuleString LSystem::generate(int iterations, bool logging) {
    RuleString current = axiom;
    
    tr1::ranlux_base_01 generator(seed);
    tr1::uniform_real<float> distribution(0.0, 1.0);

    ProductionRuleGroup matchedRules;
    Replacements replacements;
    LSystemRulesEngine engine(this);

    while (iterations--) {
        replacements.clear();
        auto currentPos = current.begin();
        while(currentPos != current.end()) {
            matchedRules.clear();
            engine.getMatchingRules(current, currentPos, matchedRules);
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
                    for (auto iter = matchedRules.rbegin(); iter != matchedRules.rend(); ++iter) {
                        s += (*iter).probability;
                        if (s > p) {
                            if (logging) {
                                cout << "Executing: " << *iter << "\n";
                            }
                            replacements.push_back(Replacement(currentPos, (*iter).successor));
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