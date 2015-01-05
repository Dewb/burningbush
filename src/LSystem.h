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
    ProductionRule(const RuleToken& pred) : predecessor(pred), probability(1.0) {}
    ProductionRule(const RuleToken& pred, const RuleString& succ) : predecessor(pred), successor(succ), probability(1.0) {}
    
    RuleToken predecessor;
    RuleString successor;
    
    RuleString leftContext;
    RuleString rightContext;
    
    float probability;
};

std::ostream& operator<<(std::ostream& os, const ProductionRule& rule);

class ProductionRuleGroup : public vector<ProductionRule> {
public:
    bool isStochastic() const { return size() > 1; }
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
    
    ProductionRule& addRule(const RuleToken& predecessor, const RuleString& successor);
    ProductionRule& addRule(const string& predecessor, const string& successor);
    ProductionRule& addRule(const char predecessor, const string& successor);
    ProductionRule& addRule(const RuleString& leftContext, const RuleToken& predecessor, const RuleString& rightContext, const RuleString& successor);
    ProductionRule& addRule(const string& leftContext, const string& predecessor, const string& rightContext, const string& successor);
    ProductionRule& addRule(const string& leftContext, const char predecessor, const string& rightContext, const string& successor);
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

template <typename T>
string to_string(T t, int precision = 2)
{
    ostringstream ss;
    ss.precision(precision);
    ss << t;
    return ss.str();
}

string to_string(const RuleString& rs);

template<typename StateType>
class TokenData {
public:
    typedef tr1::function<void(StateType&)> TokenAction;

    TokenData(RuleToken t) : token(t) { _startsGroup = false; _endsGroup = false;}
    TokenData& startsGroup() { _startsGroup = true; return *this; }
    TokenData& endsGroup() { _endsGroup = true; return *this; }
    TokenData& action(const TokenAction& action) { _actions.push_back(action); return *this; }
    
    bool shouldStartGroup() const { return _startsGroup; }
    bool shouldEndGroup() const { return _endsGroup; }
    
    const RuleToken& getToken() const { return token; }
    
    void execute(StateType& state) const { for (auto& action : _actions) { action(state); } }
protected:
    RuleToken token;
    vector<TokenAction> _actions;
    bool _startsGroup;
    bool _endsGroup;
private:
    TokenData(); // unimplemented
};

template<typename StateType>
class Generator {
public:
    typedef tr1::function<void(StateType&)> TokenAction;
    typedef TokenData<StateType> Token;
    
    void add(const Token& token) { _tokens.insert(std::pair<RuleToken, Token>(token.getToken(), token)); }
    
    const Token* getToken(const RuleToken& tokenLetter) {
        auto iter = _tokens.find(tokenLetter);
        if (iter != _tokens.end()) {
            return &(iter->second);
        }
        return NULL;
    }
    
    virtual void begin(StateType& state) {}
    virtual void end(StateType& state) {};
    
    void generate(LSystem& system, StateType& state, unsigned iterations, int steps = -1) {
        RuleString ruleStr = system.generate(iterations);
        if (steps < 0 || steps > ruleStr.size()) {
            steps = ruleStr.size();
        }
        stack<StateType> stateStack;
        stateStack.push(state);
        begin(stateStack.top());
        auto iter = ruleStr.begin();
        unsigned i = 0;
        while(i < steps && iter != ruleStr.end()) {
            auto t = getToken(*iter);
            if (t) {
                if (t->shouldStartGroup()) {
                    StateType newState = stateStack.top();
                    stateStack.push(newState);
                }
                t->execute(stateStack.top());
                if (t->shouldEndGroup() && stateStack.size() > 1) {
                    stateStack.pop();
                }
            }
            i++;
            ++iter;
        }
        end(stateStack.top());
        state = stateStack.top();
    }
    
protected:
    map<RuleToken, Token> _tokens;
};

#endif /* defined(__burningbush__LSystem__) */
