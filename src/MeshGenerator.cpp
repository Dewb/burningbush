//
//  MeshGenerator.cpp
//  burningbush
//
//  Created by Dewb on 12/28/14.
//
//

#include "MeshGenerator.h"

namespace {
    void addLineToMesh(ofMesh& mesh, const ofVec3f& pt1, const ofVec3f& pt2) {
        ofLog() << "Adding (" << pt1 << ") - (" << pt2 << ")";
        int count = mesh.getVertices().size();
        mesh.addVertex(pt1);
        mesh.addVertex(pt2);
        mesh.addIndex(count);
        mesh.addIndex(count + 1);
    }
}

// These could be anon lambdas in C++11
void f_action(ofMesh& mesh, MeshGeneratorState& state) {
    state.previousPosition = state.position;
    state.position += state.heading.normalized() * state.edgeLength;
    addLineToMesh(mesh, state.previousPosition, state.position);
}                                                                                                                                                                                                       

void plus_action(ofMesh& mesh, MeshGeneratorState& state) {
    state.heading.rotate(state.angle, ofVec3f(0, 0, 1));
}

void minus_action(ofMesh& mesh, MeshGeneratorState& state) {
    state.heading.rotate(-state.angle, ofVec3f(0, 0, 1));
}


MeshGenerator::MeshGenerator() {
    add(Token('F').action(f_action));
    add(Token('[').startsGroup());
    add(Token(']').endsGroup());
    add(Token('+').action(plus_action));
    add(Token('-').action(minus_action));
}

void MeshGenerator::begin(ofMesh& mesh) {
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_LINES);
    mesh.enableIndices();
}

void MeshGenerator::end(ofMesh& mesh) {
    mesh.mergeDuplicateVertices();
}
