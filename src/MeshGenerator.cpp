//
//  MeshGenerator.cpp
//  burningbush
//
//  Created by Dewb on 12/28/14.
//
//

#include "MeshGenerator.h"
#include <ofMain.h>


ColorBook::ColorBook() {
    colors.push_back(vector<ofColor>());
}

void ColorBook::add(const ofColor& c, int number) {
    for (int i = 0; i < number; i++) {
        colors.back().push_back(c);
    }
}

namespace {
    ofColor hsbLerp(const ofColor& c1, const ofColor& c2, float s) {
        float h1, s1, b1, h2, s2, b2;
        c1.getHsb(h1, s1, b1);
        c2.getHsb(h2, s2, b2);
        ofColor r;
        r.setHsb(h1 + s * (h2 - h1),
                 s1 + s * (s2 - s1),
                 b1 + s * (b2 - b1));
        return r;
    }
}

void ColorBook::addGradient(const ofColor& c1, const ofColor& c2, int number) {
    for (int i = 0; i < number; i++) {
        add(hsbLerp(c1, c2, i / (number - 1.0)));
    }
}

void ColorBook::addGradient(const ofColor& next, int number) {
    if (colors.back().size() > 0) {
        addGradient(colors.back().back(), next, number);
    } else {
        add(next, number);
    }
}

const ofColor& ColorBook::getColor(int series, int index) {
    if (series < colors.size() && series >= 0 &&
        index < colors[series].size() && index >= 0) {
        return colors[series][index];
    } else {
        return ofColor::black;
    }
}

void ColorBook::nextSeries() {
    colors.push_back(vector<ofColor>());
}

int ColorBook::getNumSeries() const {
    return colors.size();
}

int ColorBook::getNumColors(int series) const {
    if (series < colors.size() && series >= 0) {
        return colors[series].size();
    } else {
        return 0;
    }
}

MeshGeneratorState::MeshGeneratorState()
: heading(ofVec3f(0, 1, 0))
, up(ofVec3f(0, 0, 1))
, left(ofVec3f(1, 0, 0))
, currentColor(0)
, currentColorSeries(0)
, inPolygonNode(false)
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
    
    void forward(MeshGeneratorState& state) {
        state.position += state.heading.normalized() * state.segmentLength;
    }

    void forward_draw(MeshGeneratorState& state) {
        ofVec3f previous = state.position;
        forward(state);
        if (!state.inPolygonNode) {
            if (state.result) {
                state.result->addSegment(state.position, previous, state.segmentRadius, state.getCurrentColor());
            }
        } else {
            state.pointHistory.push_back(state.position);
            state.colorHistory.push_back(state.getCurrentColor());
        }
    }

    void turn_left(MeshGeneratorState& state) {
        state.heading.rotate(state.angle, state.up);
        state.left.rotate(state.angle, state.up);
    }

    void turn_right(MeshGeneratorState& state) {
        state.heading.rotate(-state.angle, state.up);
        state.left.rotate(-state.angle, state.up);
    }

    void pitch_down(MeshGeneratorState& state) {
        state.heading.rotate(state.angle, state.left);
        state.up.rotate(state.angle, state.left);
    }
    
    void pitch_up(MeshGeneratorState& state) {
        state.heading.rotate(-state.angle, state.left);
        state.up.rotate(-state.angle, state.left);
    }

    void roll_left(MeshGeneratorState& state) {
        state.left.rotate(state.angle, state.heading);
        state.up.rotate(state.angle, state.heading);
    }
    
    void roll_right(MeshGeneratorState& state) {
        state.left.rotate(-state.angle, state.heading);
        state.up.rotate(-state.angle, state.heading);
    }
    
    void turn_around(MeshGeneratorState& state) {
        state.heading.rotate(180, state.up);
        state.left.rotate(180, state.up);
    }
    
    void decrease_diameter(MeshGeneratorState& state) {
        state.segmentRadius *= 0.618034;
    }
    
    void increase_length(MeshGeneratorState& state) {
        state.segmentLength *= (1.0 / 0.618034);
    }
    
    void decrease_length(MeshGeneratorState& state) {
        state.segmentLength *= 0.618034;
    }
    
    void next_color(MeshGeneratorState& state) {
        state.currentColor++;
    }
    
    void previous_color(MeshGeneratorState& state) {
        state.currentColor--;
    }
    
    void next_color_series(MeshGeneratorState& state) {
        //state.currentColor = 0;
        state.currentColorSeries++;
    }

    void begin_polygon(MeshGeneratorState& state) {
        state.inPolygonNode = true;
        state.pointHistory.push_back(state.position);
        state.colorHistory.push_back(state.getCurrentColor());
    }

    void end_polygon(MeshGeneratorState& state) {
        if (state.result) {
            state.result->addPolygon(state.pointHistory, state.colorHistory);
        }
    }

}

MeshGenerator::MeshGenerator() {
    add(Symbol('F').action(forward_draw));
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
