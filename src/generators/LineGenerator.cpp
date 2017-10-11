//
//  MeshGenerator.cpp
//  burningbush
//
//  Created by Dewb on 12/28/14.
//
//

#include "LineGenerator.h"

LineGeneratorState::LineGeneratorState()
: position(ofVec2f(0, 0))
, previousPosition(ofVec2f(0, 0))
, heading(0)
, angle(90)
, edgeLength(4.0)
{
}

namespace {
    
    void forward_move(LineGeneratorState& state, FloatParams& params) {
        float length = state.edgeLength;
        if (params.size() > 0) {
            length *= params[0];
        }
        state.previousPosition = state.position;
        state.position +=
            ofVec2f(cos((state.heading - 90) * DEG_TO_RAD), sin((state.heading - 90) * DEG_TO_RAD)) * length;
    }
    
    void forward_draw(LineGeneratorState& state, FloatParams& params) {
        forward_move(state, params);
        ofLine(state.previousPosition.x, state.previousPosition.y, state.position.x, state.position.y);
    }

    void rotate_cw(LineGeneratorState& state, FloatParams& params) {
        float angle = state.angle;
        if (params.size() > 0) {
            angle = params[0];
        }
        state.heading = fmod(state.heading + angle, 360.0f);
    }

    void rotate_ccw(LineGeneratorState& state, FloatParams& params) {
        float angle = state.angle;
        if (params.size() > 0) {
            angle = params[0];
        }
        state.heading = fmod(state.heading - angle, 360.0f);
    }
    
}

LineGenerator::LineGenerator() {
    add(Symbol('F').action(forward_draw));
    add(Symbol('f').action(forward_move));
    add(Symbol('+').action(rotate_ccw));
    add(Symbol('-').action(rotate_cw));
    add(Symbol('[').startsGroup());
    add(Symbol(']').endsGroup());
}

