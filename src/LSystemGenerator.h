//
//  LSystemGenerator.h
//  burningbush
//
//  Created by Dewb on 1/5/15.
//
//

#ifndef burningbush_LSystemGenerator_h
#define burningbush_LSystemGenerator_h

#include "LSystem.h"

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

#endif
