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
#include <stack>
#include <vector>
#include <ofMesh.h>

using namespace std;

typedef string Rule;
typedef set<pair<Rule, Rule> > RuleSet;
typedef pair<ofVec3f, ofVec3f> HistoryItem;
typedef stack<HistoryItem> History;

class LSystem {
public:
    Rule axiom;
    RuleSet rules;
    ofMesh mesh;
    
    float edgeLength;
    float angle;
    
    void generate(int n);
protected:
    
    void generateMesh(const Rule& rule);
};

#endif /* defined(__burningbush__LSystem__) */
