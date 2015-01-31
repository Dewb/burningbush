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
    RuleToken(const char* str);
    RuleToken(const char c);
    RuleToken(const string& str);
    
    string symbol;
    string subscript;
    vector<string> parameters;

    bool operator<(const RuleToken& rhs) const;
    bool operator>(const RuleToken& rhs) const;
    bool operator==(const RuleToken& rhs) const;
    bool operator!=(const RuleToken& rhs) const;
    bool operator==(const string& rhs) const;
    bool operator==(const char& rhs) const;
    
    bool isParametric() const { return parameters.size() > 0; }
};

class RuleString : public list<RuleToken> {
public:
    RuleString() {}
    RuleString(const string& s);
    RuleString(const char* c);
};

class ProductionRule {
public:
    ProductionRule(const RuleToken& pred, const RuleString& succ) : predecessor(pred), successor(succ), probability(1.0) {}
    
    RuleToken predecessor;
    RuleString successor;
    
    RuleString leftContext;
    RuleString rightContext;
    
    string parametricCondition;
    
    float probability;
    
    bool isStochastic() const   {
        return probability < 1.0;
    }
    bool isParametric() const {
        return predecessor.isParametric();
    }
    ProductionRule& setContext(const string& left, const string& right) {
        leftContext = left;
        rightContext = right;
        return *this;
    }
    ProductionRule& setLeftContext(const string& context) { leftContext = context; return *this; }
    ProductionRule& setRightContext(const string& context) { rightContext = context; return *this; }
    ProductionRule& setCondition(const string& condition) { parametricCondition = condition; return *this; }
    ProductionRule& setProbability(float p) { probability = p; return *this; }
};

std::ostream& operator<<(std::ostream& os, const ProductionRule& rule);

class ProductionRuleGroup : public vector<ProductionRule> {
public:
    bool isStochastic() const;
};

typedef vector<pair<int, ProductionRule> > IndexedProductionRuleGroup;

typedef map<RuleToken, ProductionRuleGroup> RuleSet;

class LSystem {
public:
    LSystem();
    
    void setAxiom(const RuleString& axiomString);
    const RuleString& getAxiom() const { return axiom; }
    
    void ignoreForContext(const RuleString& ignoreString);
    
    ProductionRule& addRule(const RuleToken& predecessor, const RuleString& successor);
    ProductionRule& addRule(const RuleString& leftContext, const RuleToken& predecessor, const RuleString& rightContext, const RuleString& successor);
    const RuleSet& getRules() const { return rules; }
    
    RuleString generate(int iteration, bool logging = false);
    
    void reset();
    void reseed(unsigned seed);
    void reseed();
    unsigned getSeed() const { return seed; }
    bool isStochastic() const;
    
    void setProperty(string name, float value);
    float getProperty(string name) const;
    bool hasProperty(string name) const;
    
    void setTitle(const string& strTitle) { title = strTitle; }
    const string& getTitle() const { return title; }
protected:
    RuleString axiom;
    RuleSet rules;
    RuleString ignoreContext;
    
    map<string, float> properties;
    unsigned seed;
    string title;
    
    friend class LSystemRulesEngine;
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
