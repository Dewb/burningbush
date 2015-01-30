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

class LSystemRulesEngine {
public:
    LSystemRulesEngine(LSystem* ls) : system(ls) {}
    void getMatchingRules(const RuleString& current,
                          const RuleString::iterator& currentPos,
                          ProductionRuleGroup& matched) const;
protected:
    LSystem* system;
private:
    LSystemRulesEngine();
};

#endif /* defined(__burningbush__LSystemRulesEngine__) */
