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
#include <map>
#include <set>
#include <vector>
#include <list>
#include <sstream>

using namespace std;


class RuleToken {
public:
    RuleToken(const string& str, int* pParsePosition = NULL);
    RuleToken(const char* str);
    RuleToken(const char c);
    
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

std::ostream& operator<<(std::ostream& os, const RuleToken& rule);
string to_string(const RuleToken& rt);


class RuleString : public list<RuleToken> {
public:
    RuleString() {}
    RuleString(const string& s);
    RuleString(const char* c);
};

std::ostream& operator<<(std::ostream& os, const RuleString& rule);
string to_string(const RuleString& rs);


class ProductionRule {
public:
    ProductionRule(const RuleToken& pred, const RuleString& succ);
    
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
    
    ProductionRule& setContext(const string& left, const string& right);
    ProductionRule& setLeftContext(const string& context);
    ProductionRule& setRightContext(const string& context);
    ProductionRule& setCondition(const string& condition);
    ProductionRule& setProbability(float p);
};

std::ostream& operator<<(std::ostream& os, const ProductionRule& rule);
string to_string(const ProductionRule& prod);

class ProductionRuleGroup : public vector<ProductionRule> {
public:
    bool isStochastic() const;
};

typedef vector<pair<int, ProductionRule> > IndexedProductionRuleGroup;
typedef map<RuleToken, ProductionRuleGroup> RuleSet;


typedef pair<int, RuleString> ResultsCacheLine;
struct ResultsCacheComparator {
    bool operator() (const ResultsCacheLine& lhs, const ResultsCacheLine& rhs) const {
        return lhs.first > rhs.first;
    }
};

typedef set<ResultsCacheLine, ResultsCacheComparator> ResultsCache;

class LSystemOptions {
public:
    bool logging;
    bool useCache;
    int steps;

    LSystemOptions();

    static LSystemOptions DefaultOptions;
};


class LSystem {
public:
    LSystem();
    
    void setAxiom(const RuleString& axiomString);
    const RuleString& getAxiom() const { return axiom; }
    
    void ignoreForContext(const RuleString& ignoreString);
    
    ProductionRule& addRule(const RuleToken& predecessor, const RuleString& successor);
    ProductionRule& addRule(const RuleString& leftContext, const RuleToken& predecessor, const RuleString& rightContext, const RuleString& successor);
    const RuleSet& getRules() const { return rules; }
    
    RuleString generate(int iteration, LSystemOptions& options = LSystemOptions::DefaultOptions);
    
    void reset();
    void reseed(unsigned seed);
    void reseed();
    unsigned getSeed() const { return seed; }
    bool isStochastic() const;
    
    void setProperty(string name, float value);
    float getProperty(string name) const;
    bool hasProperty(string name) const;
    const vector<string> getProperties() const;
    
    void setTitle(const string& strTitle) { title = strTitle; }
    const string& getTitle() const { return title; }
protected:
    RuleString axiom;
    RuleSet rules;
    RuleString ignoreContext;
    
    map<string, float> properties;
    unsigned seed;
    string title;

    ResultsCache cache;
    
    friend class LSystemRulesEngine;
};

template <typename T>
string to_string(T t, int precision = 2)
{
    ostringstream ss;
    ss.precision(precision);
    ss << t;
    return ss.str();
}


#endif /* defined(__burningbush__LSystem__) */
