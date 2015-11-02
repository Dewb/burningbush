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
#include <tr1/functional>
#include <stack>
#include <iostream>

typedef vector<float> FloatParams;

template<typename StateType>
class SymbolData {
public:
    typedef tr1::function<void(StateType&, FloatParams&)> SymbolAction;
    
    SymbolData(RuleToken t) : token(t) { _startsGroup = false; _endsGroup = false;}
    SymbolData& startsGroup() { _startsGroup = true; return *this; }
    SymbolData& endsGroup() { _endsGroup = true; return *this; }
    SymbolData& action(const SymbolAction& action) { _actions.push_back(action); return *this; }
    
    bool shouldStartGroup() const { return _startsGroup; }
    bool shouldEndGroup() const { return _endsGroup; }
    
    const RuleToken& getToken() const { return token; }
    
    void execute(StateType& state, FloatParams& params) const { for (auto& action : _actions) { action(state, params); } }
protected:
    RuleToken token;
    vector<SymbolAction> _actions;
    bool _startsGroup;
    bool _endsGroup;
private:
    SymbolData(); // unimplemented
};

template<typename StateType>
class Generator {
public:
    typedef tr1::function<void(StateType&, FloatParams)> SymbolAction;
    typedef SymbolData<StateType> Symbol;
    
    void add(const Symbol& token) { _symbols.insert(std::pair<RuleToken, Symbol>(token.getToken(), token)); }
    
    const Symbol* getSymbol(const RuleToken& token) {
        for (auto iter = _symbols.begin(); iter != _symbols.end(); iter++) {
            if (token.symbol == iter->first.symbol) {
                return &(iter->second);
            }
        }
        return NULL;
    }

    struct StackFrame {
        StateType state;
        RuleString::const_iterator groupStartPoint;
        int groupRepeatCount;
        StackFrame(StateType& s, RuleString::const_iterator groupStart, int groupRepeat) : state(s), groupStartPoint(groupStart), groupRepeatCount(groupRepeat) {}
    };
    
    virtual void begin(StateType& state) {}
    virtual void end(StateType& state) {};
    
    void generate(LSystem& system, StateType& state, unsigned iterations, LSystemOptions& options) {
        RuleString ruleStr = system.generate(iterations, options);
        int steps = options.steps;
        if (steps < 0 || steps > ruleStr.size()) {
            steps = INT_MAX;
        }

        RuleString::const_iterator iter = ruleStr.begin();

        stack<StackFrame> stateStack;
        stateStack.push(StackFrame(state, ruleStr.end(), 0));

        begin(stateStack.top().state);

        unsigned i = 0;
        while(i < steps && iter != ruleStr.end()) {
            auto sym = getSymbol(*iter);
            if (sym) {
                if (sym->shouldStartGroup()) {
                    stateStack.push(StackFrame(stateStack.top().state, iter, 0));
                }
                
                FloatParams floatParams;
                for (auto& stringParam : iter->parameters) {
                    stringstream ss(stringParam);
                    float x;
                    ss >> x;
                    if (ss.fail()) {
                        cout << "ERROR: parameter " << stringParam << " is non-numeric!\n";
                    } else {
                        floatParams.push_back(x);
                    }
                }
                
                sym->execute(stateStack.top().state, floatParams);
                
                if (sym->shouldEndGroup() && stateStack.size() > 1) {

                    if (floatParams.size() > 0 && stateStack.top().groupStartPoint != ruleStr.end()) {
                        int totalCopies = (int)ceil(floatParams[0]);
                        if (stateStack.top().groupRepeatCount < totalCopies - 1) {
                            // starting to repeat
                            iter = stateStack.top().groupStartPoint;
                            stateStack.top().groupRepeatCount++;
                        } else {
                            stateStack.pop();
                        }

                    } else {
                        stateStack.pop();
                    }
                }
            }
            i++;
            ++iter;
        }
        end(stateStack.top().state);
        state = stateStack.top().state;
    }
    
protected:
    map<RuleToken, Symbol> _symbols;
};

#endif
