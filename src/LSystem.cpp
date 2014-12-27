//
//  LSystem.cpp
//  burningbush
//
//  Created by Dewb on 12/26/14.
//
//

#include "LSystem.h"

namespace {
    void addLineToMesh(ofMesh& mesh, const ofVec3f& pt1, const ofVec3f& pt2) {
        ofLog() << "Adding (" << pt1 << ") - (" << pt2 << ")";
        int count = mesh.getVertices().size();
        mesh.addVertex(pt1);
        mesh.addVertex(pt2);
        mesh.addIndex(count);
        mesh.addIndex(count + 1);
    }
    
    void find_and_replace(std::string& str, const std::string& findStr, const std::string& replaceStr)
    {
        size_t pos = 0;
        while((pos = str.find(findStr, pos)) != std::string::npos)
        {
            str.replace(pos, findStr.length(), replaceStr);
            pos += replaceStr.length();
        }
    }
}


void LSystem::generate(int iterations) {
    
    mesh.setMode(OF_PRIMITIVE_LINES);
    
    Rule current = axiom;
    
    while (iterations--) {
        for (auto iter = rules.rbegin(); iter != rules.rend(); iter++) {
            auto& rule = *iter;
        //for (auto& rule : rules) {
            ofLog() << "Executing rule " << rule.first << " -> " << rule.second;
            find_and_replace(current, rule.first, rule.second);
            ofLog() << "  " << current;
        }
    }
    
    generateMesh(current);
}

void LSystem::generateMesh(const Rule& current) {
    
    mesh.clear();
    mesh.enableIndices();
    
    ofVec3f turtleHeading(0, 1, 0);
    ofVec3f turtlePos(0, 0, 0);
    ofVec3f previousPos(0, 0, 0);
    History history;

    for (auto& symbol : current) {
        previousPos = turtlePos;
        switch (symbol) {
            case 'F':
                turtlePos += turtleHeading.normalized() * edgeLength;
                addLineToMesh(mesh, previousPos, turtlePos);
                break;
            case '[':
                history.push(HistoryItem(turtlePos, turtleHeading));
                break;
            case ']':
                turtlePos = history.top().first;
                turtleHeading = history.top().second;
                history.pop();
                break;
            case '+':
                turtleHeading.rotate(angle, ofVec3f(0, 0, 1));
                break;
            case '-':
                turtleHeading.rotate(-angle, ofVec3f(0, 0, 1));
                break;
        }
    }
    
    mesh.mergeDuplicateVertices();
}