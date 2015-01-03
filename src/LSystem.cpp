//
//  LSystem.cpp
//  burningbush
//
//  Created by Dewb on 12/26/14.
//
//

#include "LSystem.h"
#include <tr1/tuple>


void LSystem::addRule(const Rule& match, const Rule& replace) {
    rules.push_back(std::pair<Rule, Rule>(match, replace));
}

void LSystem::reset() {
    rules.clear();
    properties.clear();
    axiom = "";
}

Rule LSystem::generate(int iterations) {
    Rule current = axiom;
   
    while (iterations--) {
        size_t position = 0;
        while(position < current.length()) {
            for (auto iter = rules.begin(); iter != rules.end(); ++iter) {
                if (current.substr(position, 1) == iter->first) {
                    current.replace(position, iter->first.length(), iter->second);
                    position += iter->second.length() - 1;
                    break;
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