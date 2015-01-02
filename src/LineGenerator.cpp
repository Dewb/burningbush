//
//  MeshGenerator.cpp
//  burningbush
//
//  Created by Dewb on 12/28/14.
//
//

#include "LineGenerator.h"

// These should be anon lambdas when oF supports C++11
namespace {
    void forward(LineGeneratorState& state) {
        state.previousPosition = state.position;
        state.position += state.heading.normalized() * state.edgeLength;
        //ofPushStyle();
        //ofSetColor(255, 0, 0, 128);
        //ofLine(state.previousPosition.x, state.previousPosition.y, state.position.x, state.position.y);
        //ofPopStyle();
    }
    
    void forward_draw(LineGeneratorState& state) {
        state.previousPosition = state.position;
        state.position += state.heading.normalized() * state.edgeLength;
        ofLine(state.previousPosition.x, state.previousPosition.y, state.position.x, state.position.y);
    }

    void rotate_cw(LineGeneratorState& state) {
        state.heading.rotate(state.angle);
    }

    void rotate_ccw(LineGeneratorState& state) {
        state.heading.rotate(-state.angle);
    }
}

LineGenerator::LineGenerator() {
    add(Token('F').action(forward_draw));
    add(Token('f').action(forward));
    add(Token('+').action(rotate_cw));
    add(Token('-').action(rotate_ccw));
    add(Token('[').startsGroup());
    add(Token(']').endsGroup());
}

