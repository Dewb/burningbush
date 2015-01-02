//
//  MeshGenerator.cpp
//  burningbush
//
//  Created by Dewb on 12/28/14.
//
//

#include "MeshGenerator.h"

namespace {
    void addLineToMesh(ofPtr<ofMesh> mesh, const ofVec3f& pt1, const ofVec3f& pt2) {
        ofLog() << "Adding (" << pt1 << ") - (" << pt2 << ")";
        int count = mesh->getVertices().size();
        mesh->addVertex(pt1);
        mesh->addVertex(pt2);
        mesh->addIndex(count);
        mesh->addIndex(count + 1);
    }
}

// These could be anon lambdas in C++11
namespace fn {
    void f(MeshGeneratorState& state) {
        state.previousPosition = state.position;
        state.position += state.heading.normalized() * state.edgeLength;
        addLineToMesh(state.mesh, state.previousPosition, state.position);
    }                                                                                                                                                                                                       

    void plus(MeshGeneratorState& state) {
        state.heading.rotate(state.angle, ofVec3f(0, 0, 1));
    }

    void minus(MeshGeneratorState& state) {
        state.heading.rotate(-state.angle, ofVec3f(0, 0, 1));
    }
}

MeshGenerator::MeshGenerator() {
    add(Token('F').action(fn::f));
    add(Token('[').startsGroup());
    add(Token(']').endsGroup());
    add(Token('+').action(fn::plus));
    add(Token('-').action(fn::minus));
}

void MeshGenerator::begin(MeshGeneratorState& state) {
    state.mesh.reset(new ofMesh());
    state.mesh->setMode(OF_PRIMITIVE_LINES);
    state.mesh->enableIndices();
}

void MeshGenerator::end(MeshGeneratorState& state) {
    state.mesh->mergeDuplicateVertices();
}
