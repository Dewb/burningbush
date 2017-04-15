//
//  LSystemRulesEngine.h
//  burningbush
//
//  Created by Michael Dewberry on 1/29/15.
//
//

#ifndef __burningbush__LSystemRulesEngine__
#define __burningbush__LSystemRulesEngine__

#include "LSystem.h"

class ExpressionCache;

typedef vector<pair<int, ProductionRule*> > IndexedProductionRulePtrGroup;

class LSystemRulesEngine {
public:
    LSystemRulesEngine(LSystem* ls);
    ~LSystemRulesEngine();
    
    void getMatchingRules(const RuleString& current,
                          const RuleString::iterator& currentPos,
                          IndexedProductionRulePtrGroup& matched);
    RuleString evaluateSuccessor(int ruleIndex,
                                 const RuleToken& predecessor,
                                 const RuleToken& tokenMatch,
                                 const RuleString& successor);
protected:
    LSystem* system;
    ExpressionCache* expressionCache;
private:
    LSystemRulesEngine();
};

#endif /* defined(__burningbush__LSystemRulesEngine__) */
