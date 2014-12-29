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
#include <ofTypes.h>

using namespace std;


typedef string Rule;
typedef char RuleToken;
typedef set<pair<Rule, Rule> > RuleSet;

class LSystem {
public:
    Rule axiom;
    RuleSet rules;
    void addRule(const Rule& match, const Rule& replace);
    Rule generate(int iteration);
};


template<typename ResultType, typename StateType>
class TokenData {
public:
    typedef tr1::function<void(ResultType&, StateType&)> TokenAction;

    TokenData(RuleToken t) { token = t; _startsGroup = false; _endsGroup = false;}
    TokenData& startsGroup() { _startsGroup = true; return *this; }
    TokenData& endsGroup() { _endsGroup = true; return *this; }
    TokenData& action(const TokenAction& action) { _actions.push_back(action); return *this; }
    
    bool shouldStartGroup() const { return _startsGroup; }
    bool shouldEndGroup() const { return _endsGroup; }
    
    const wchar_t getToken() const { return token; }
    
    void execute(ResultType& result, StateType& state) const { for (auto& action : _actions) { action(result, state); } }
protected:
    RuleToken token;
    vector<TokenAction> _actions;
    bool _startsGroup;
    bool _endsGroup;
private:
    TokenData(); // unimplemented
};


template<typename ResultType, typename StateType>
class Generator {
public:
    
    typedef tr1::function<void(ResultType&, StateType&)> TokenAction;
    typedef TokenData<ResultType, StateType> Token;
    
    Generator() { _system.reset(new LSystem()); }
    
    void setInitialState(StateType state) { _stateStack.push(state); }
    void add(const Token& token) { _tokens.insert(std::pair<wchar_t, Token>(token.getToken(), token)); }
    
    const LSystem& getSystem() const { return _system; }
    LSystem& getSystem() { return *(_system.get()); }
    
    const Token* getToken(wchar_t tokenLetter) {
        auto iter = _tokens.find(tokenLetter);
        if (iter != _tokens.end()) {
            return &(iter->second);
        }
        return NULL;
    }
    
    virtual void begin(ResultType& r) {}
    virtual void end(ResultType& r) {};
    
    ResultType generate(int iterations) {
        size_t n = _system->generate(iterations).size();
        return generate(iterations, n);
    }
    
    ResultType generate(int iterations, int steps) {
        ResultType r;
        Rule str = _system->generate(iterations);
        begin(r);
        if (_stateStack.size() != 1) {
            // did not call setInitialState!
            return;
        }
        for (int i = 0; i < steps; i++) {
            auto t = getToken(str[i]);
            if (t) {
                if (t->shouldStartGroup()) {
                    _stateStack.push(_stateStack.top());
                }
                t->execute(r, _stateStack.top());
                if (t->shouldEndGroup() && _stateStack.size() > 1) {
                    _stateStack.pop();
                }
            }
        }
        end(r);
        return r;
    }
    
protected:
    
    ofPtr<LSystem> _system;
    stack<StateType> _stateStack;
    map<wchar_t, Token> _tokens;
    
};

#endif /* defined(__burningbush__LSystem__) */
