//
//  MeshGenerator.cpp
//  burningbush
//
//  Created by Dewb on 12/28/14.
//
//

#include "LineGenerator.h"

// These could be anon lambdas in C++11
namespace fn {
    void f(LineGeneratorState& state) {
        state.previousPosition = state.position;
        state.position += state.heading.normalized() * state.edgeLength;
        ofLine(state.previousPosition.x, state.previousPosition.y, state.position.x, state.position.y);
    }

    void plus(LineGeneratorState& state) {
        state.heading.rotate(state.angle);
    }

    void minus(LineGeneratorState& state) {
        state.heading.rotate(-state.angle);
    }
}

LineGenerator::LineGenerator() {
    add(Token('F').action(fn::f));
    add(Token('[').startsGroup());
    add(Token(']').endsGroup());
    add(Token('+').action(fn::plus));
    add(Token('-').action(fn::minus));
}

