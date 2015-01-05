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
#include <tr1/functional>

using namespace std;

typedef char RuleToken;
typedef basic_string<RuleToken> RuleString;

class ProductionRule {
public:
    ProductionRule() : probability(1.0) {}
    
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
    
    RuleString axiom;
    RuleSet rules;
    RuleString ignoreContext;
    
    ProductionRule& addRule(const RuleToken& match, const RuleString& replace);
    void addRule(const RuleString& leftContext, const RuleToken& match, const RuleString& rightContext, const RuleString& replace);
    void reset();
    
    RuleString generate(int iteration, bool logging = true);

    void reseed(unsigned seed);
    void reseed();
    bool isStochastic() const;
    
    void setProperty(string name, float value);
    float getProperty(string name) const;
    bool hasProperty(string name) const;

protected:
    bool leftContextMatches(const RuleString& leftContext, const RuleString& current, int position) const;
    bool rightContextMatches(const RuleString& rightContext, const RuleString& current, int position) const;
    void getMatchingRules(const RuleString& current, int position, ProductionRuleGroup& matched);
    
    map<string, float> properties;
    unsigned seed;
};

template<typename StateType>
class TokenData {
public:
    typedef tr1::function<void(StateType&)> TokenAction;

    TokenData(RuleToken t) { token = t; _startsGroup = false; _endsGroup = false;}
    TokenData& startsGroup() { _startsGroup = true; return *this; }
    TokenData& endsGroup() { _endsGroup = true; return *this; }
    TokenData& action(const TokenAction& action) { _actions.push_back(action); return *this; }
    
    bool shouldStartGroup() const { return _startsGroup; }
    bool shouldEndGroup() const { return _endsGroup; }
    
    const wchar_t getToken() const { return token; }
    
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
    
    void add(const Token& token) { _tokens.insert(std::pair<wchar_t, Token>(token.getToken(), token)); }
    
    const Token* getToken(wchar_t tokenLetter) {
        auto iter = _tokens.find(tokenLetter);
        if (iter != _tokens.end()) {
            return &(iter->second);
        }
        return NULL;
    }
    
    virtual void begin(StateType& state) {}
    virtual void end(StateType& state) {};
    
    void generate(LSystem& system, StateType& state, unsigned iterations, unsigned steps = -1) {
        RuleString str = system.generate(iterations);
        if (steps < 0 || steps > str.size()) {
            steps = str.size();
        }
        stack<StateType> stateStack;
        stateStack.push(state);
        begin(stateStack.top());
        for (unsigned i = 0; i < steps; i++) {
            auto t = getToken(str[i]);
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
        }
        end(stateStack.top());
        state = stateStack.top();
    }
    
protected:
    map<wchar_t, Token> _tokens;
};

#endif /* defined(__burningbush__LSystem__) */
