//
//  LSystem.cpp
//  burningbush
//
//  Created by Dewb on 12/26/14.
//
//

#include "LSystem.h"

namespace {
    void find_and_replace(Rule& str, const Rule& findStr, const Rule& replaceStr) {
        size_t pos = 0;
        while((pos = str.find(findStr, pos)) != std::string::npos) {
            str.replace(pos, findStr.length(), replaceStr);
            pos += replaceStr.length();
        }
    }
}

void LSystem::addRule(const Rule& match, const Rule& replace) {
    rules.insert(std::pair<Rule, Rule>(match, replace));
}

Rule LSystem::generate(int iterations) {
    Rule current = axiom;
    while (iterations--) {
        for (auto iter = rules.rbegin(); iter != rules.rend(); ++iter) {
            find_and_replace(current, iter->first, iter->second);
        }
    }
    return current;
}

