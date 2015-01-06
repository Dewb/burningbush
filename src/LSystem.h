//
//  LSystem.h
//  burningbush
//
//  Created by Dewb on 12/26/14.
//
//

#ifndef __burningbush__LSystem__
#define __burningbush__LSystem__

#include <string>
#include <set>
#include <map>
#include <stack>
#include <vector>
#include <list>
#include <tr1/functional>
#include <sstream>

using namespace std;

class RuleToken {
public:
    RuleToken(char c) { symbol.push_back(c); }
    RuleToken(string sym) { symbol = sym; }
    string symbol;
    string subscript;

    bool operator<(const RuleToken& rhs) const;
    bool operator>(const RuleToken& rhs) const;
    bool operator==(const RuleToken& rhs) const;
    bool operator!=(const RuleToken& rhs) const;
    bool operator==(const string& rhs) const;
    bool operator==(const char& rhs) const;
};
typedef list<RuleToken> RuleString;

class ProductionRule {
public:
    ProductionRule(const RuleToken& pred, const RuleString& succ) : predecessor(pred), successor(succ), probability(1.0) {}
    
    RuleToken predecessor;
    RuleString successor;
    
    RuleString leftContext;
    RuleString rightContext;
    
    float probability;
    
    bool isStochastic() const   {
        return probability < 1.0;
    }
};

std::ostream& operator<<(std::ostream& os, const ProductionRule& rule);

class ProductionRuleGroup : public vector<ProductionRule> {
public:
    bool isStochastic() const;
};

typedef map<RuleToken, ProductionRuleGroup> RuleSet;

class LSystem {
public:
    LSystem();
    
    void setAxiom(const RuleString& axiomString);
    void setAxiom(const string& axiomString);
    const RuleString& getAxiom() const { return axiom; }
    
    void ignoreForContext(const RuleString& ignoreString);
    void ignoreForContext(const string& ignoreString);
    
    ProductionRule& addRule(const RuleToken& predecessor, const RuleString& successor, float prob = 1.0);
    ProductionRule& addRule(const string& predecessor, const string& successor, float prob = 1.0);
    ProductionRule& addRule(const char predecessor, const string& successor, float prob = 1.0);
    ProductionRule& addRule(const RuleString& leftContext, const RuleToken& predecessor, const RuleString& rightContext, const RuleString& successor, float prob = 1.0);
    ProductionRule& addRule(const string& leftContext, const string& predecessor, const string& rightContext, const string& successor, float prob = 1.0);
    ProductionRule& addRule(const string& leftContext, const char predecessor, const string& rightContext, const string& successor, float prob = 1.0);
    const RuleSet& getRules() const { return rules; }
    
    RuleString generate(int iteration, bool logging = true);
    
    void reset();
    void reseed(unsigned seed);
    void reseed();
    bool isStochastic() const;
    
    void setProperty(string name, float value);
    float getProperty(string name) const;
    bool hasProperty(string name) const;

protected:
    RuleString axiom;
    RuleSet rules;
    RuleString ignoreContext;
    
    template <typename Iter>
    bool contextMatches(const Iter& contextBegin, const Iter& contextEnd,
                        const Iter& stringBegin, const Iter& stringEnd,
                        bool reversed, bool followBranches) const;
    void getMatchingRules(const RuleString& current, const RuleString::iterator& currentPos, ProductionRuleGroup& matched);
    
    map<string, float> properties;
    unsigned seed;
};

string to_string(const ProductionRule& prod);
string to_string(const RuleToken& rt);
string to_string(const RuleString& rs);

template <typename T>
string to_string(T t, int precision = 2)
{
    ostringstream ss;
    ss.precision(precision);
    ss << t;
    return ss.str();
}


#endif /* defined(__burningbush__LSystem__) */
