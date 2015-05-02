//
//  MeshGenerator.cpp
//  burningbush
//
//  Created by Dewb on 12/28/14.
//
//

#include "MeshGenerator.h"
#include <ofMain.h>


MeshGeneratorState::MeshGeneratorState()
: heading(ofVec3f(0, 1, 0))
, up(ofVec3f(0, 0, 1))
, left(ofVec3f(1, 0, 0))
, currentColor(0)
, currentColorSeries(0)
, inPolygonNode(false)
, tropism(0)
{
}

MeshGeneratorState::MeshGeneratorState(const MeshGeneratorState& other) {
    position = other.position;
    heading = other.heading;
    up = other.up;
    left = other.left;
    angle = other.angle;
    segmentLength = other.segmentLength;
    segmentRadius = other.segmentRadius;
    colorBook = other.colorBook;
    currentColor = other.currentColor;
    currentColorSeries = other.currentColorSeries;
    result = other.result;
    tropism = other.tropism;
    // pointHistory not copied
    // colorHistory not copied
    inPolygonNode = false;
}

ofColor MeshGeneratorState::getCurrentColor() {
    if (colorBook) {
        int series = currentColorSeries % colorBook->getNumSeries();
        int color = currentColor % colorBook->getNumColors(series);
        return colorBook->getColor(series, color);
    } else {
        return ofColor(128, 128, 128);
    }
}


namespace {
    
    void forward(MeshGeneratorState& state, FloatParams& params) {
        float length = state.segmentLength;
        if (params.size() > 0) {
            length *= params[0];
        }

        if (!state.inPolygonNode && state.tropism > 0) {
            ofVec3f T(-0.27, -0.60, 0.00);
            float a = state.tropism * state.heading.crossed(T).length();
            state.heading.rotate(a * RAD_TO_DEG, state.up);
            state.left.rotate(a * RAD_TO_DEG, state.up);
        }

        state.position += state.heading.normalized() * length;
    }

    void forward_radius(MeshGeneratorState& state, FloatParams&) {
        state.position += state.heading.normalized() * state.segmentRadius;
    }

    void forward_draw(MeshGeneratorState& state, FloatParams& params) {
        ofVec3f previous = state.position;
        forward(state, params);
        if (!state.inPolygonNode) {
            if (state.result) {
                state.result->addSegment(state.position, previous, state.segmentRadius, state.getCurrentColor());
            }
        } else {
            state.pointHistory.push_back(state.position);
            state.colorHistory.push_back(state.getCurrentColor());
        }
    }

    void turn_left(MeshGeneratorState& state, FloatParams& params) {
        float angle = state.angle;
        if (params.size() > 0) {
            angle = params[0];
        }
        state.heading.rotate(angle, state.up);
        state.left.rotate(angle, state.up);
    }

    void turn_right(MeshGeneratorState& state, FloatParams& params) {
        float angle = state.angle;
        if (params.size() > 0) {
            angle = params[0];
        }
        state.heading.rotate(-angle, state.up);
        state.left.rotate(-angle, state.up);
    }

    void pitch_down(MeshGeneratorState& state, FloatParams& params) {
        float angle = state.angle;
        if (params.size() > 0) {
            angle = params[0];
        }
        state.heading.rotate(angle, state.left);
        state.up.rotate(angle, state.left);
    }
    
    void pitch_up(MeshGeneratorState& state, FloatParams& params) {
        float angle = state.angle;
        if (params.size() > 0) {
            angle = params[0];
        }
        state.heading.rotate(-angle, state.left);
        state.up.rotate(-angle, state.left);
    }

    void roll_left(MeshGeneratorState& state, FloatParams& params) {
        float angle = state.angle;
        if (params.size() > 0) {
            angle = params[0];
        }
        state.left.rotate(angle, state.heading);
        state.up.rotate(angle, state.heading);
    }
    
    void roll_right(MeshGeneratorState& state, FloatParams& params) {
        float angle = state.angle;
        if (params.size() > 0) {
            angle = params[0];
        }
        state.left.rotate(-angle, state.heading);
        state.up.rotate(-angle, state.heading);
    }
    
    void turn_around(MeshGeneratorState& state, FloatParams&) {
        state.heading.rotate(180, state.up);
        state.left.rotate(180, state.up);
    }
    
    void decrease_diameter(MeshGeneratorState& state, FloatParams& params) {
        if (params.size() > 0) {
            state.segmentRadius = params[0] * 0.2;
        } else {
            float factor = 0.618034;
            state.segmentRadius *= factor;
        }
    }
    
    void increase_length(MeshGeneratorState& state, FloatParams& params) {
        state.segmentLength *= (1.0 / 0.618034);
    }
    
    void decrease_length(MeshGeneratorState& state, FloatParams& params) {
        float factor = 0.618034;
        if (params.size() > 0) {
            factor = params[0];
        }
        state.segmentLength *= factor;
    }
    
    void next_color(MeshGeneratorState& state, FloatParams&) {
        state.currentColor++;
    }
    
    void previous_color(MeshGeneratorState& state, FloatParams&) {
        state.currentColor--;
    }
    
    void next_color_series(MeshGeneratorState& state, FloatParams&) {
        //state.currentColor = 0;
        state.currentColorSeries++;
    }

    void begin_polygon(MeshGeneratorState& state, FloatParams&) {
        state.inPolygonNode = true;
        state.pointHistory.push_back(state.position);
        state.colorHistory.push_back(state.getCurrentColor());
    }

    void end_polygon(MeshGeneratorState& state, FloatParams&) {
        if (state.result) {
            state.result->addPolygon(state.pointHistory, state.colorHistory);
        }
    }

    void tropism(MeshGeneratorState& state, FloatParams& params) {
        if (params.size() > 0) {
            state.tropism = params[0];
        }
    }

}

MeshGenerator::MeshGenerator() {
    add(Symbol('F').action(forward_draw));
    add(Symbol('G').action(forward_draw));
    add(Symbol('g').action(forward_radius));
    add(Symbol('T').action(tropism));
    add(Symbol('f').action(forward));
    add(Symbol('+').action(turn_left));
    add(Symbol('-').action(turn_right));
    add(Symbol('&').action(pitch_down));
    add(Symbol('^').action(pitch_up));
    add(Symbol('\\').action(roll_left));
    add(Symbol('/').action(roll_right));
    add(Symbol('|').action(turn_around));
    add(Symbol('!').action(decrease_diameter));
    add(Symbol('@').action(decrease_length));
    add(Symbol('#').action(increase_length));
    add(Symbol('\'').action(next_color));
    add(Symbol('`').action(previous_color));
    add(Symbol('"').action(next_color_series));
    add(Symbol('[').startsGroup());
    add(Symbol(']').endsGroup());
    add(Symbol('{').startsGroup().action(begin_polygon));
    add(Symbol('}').endsGroup().action(end_polygon));
}

void MeshGenerator::begin(MeshGeneratorState& state) {
    if (state.result) {
        state.result->begin();
    }
}

void MeshGenerator::end(MeshGeneratorState& state) {
    if (state.result) {
        state.result->end();
    }
}
