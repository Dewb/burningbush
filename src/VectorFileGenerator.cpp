//
//  MeshGenerator.cpp
//  burningbush
//
//  Created by Dewb on 12/28/14.
//
//

#include "VectorFileGenerator.h"
#include "ofxVectorGraphics.h"

ofxVectorGraphics vectorGfx;


#define EPSILON 1e-7


bool ofVec2dShouldSortBefore(const ofVec2d& l, const ofVec2d& r) {
    bool xEq = fabs(l.x - r.x) < EPSILON;
    bool yEq = fabs(l.y - r.y) < EPSILON;
    
    if(!xEq && l.x < r.x) return true;
    if(!xEq && l.x > r.x) return false;
    if(!yEq && l.y < r.y) return true;
    if(!yEq && l.y > r.y) return false;
    return false;
}

Segment::Segment(const ofVec2d& l, const ofVec2d& r) {
    ofVec2d a, b;
    if (ofVec2dShouldSortBefore(l, r)) {
        p1 = l;
        p2 = r;
    } else {
        p1 = r;
        p2 = l;
    }
}

std::ostream& operator<<(std::ostream& os, const Segment& seg) {
    os << seg.p1.x << ", " << seg.p1.y << " - " << seg.p2.x << ", " << seg.p2.y;
    return os;
}

bool SegmentCompare::operator()(const Segment& l, const Segment& r) const
{
    bool p1xEq = fabs(l.p1.x - r.p1.x) < EPSILON;
    bool p1yEq = fabs(l.p1.y - r.p1.y) < EPSILON;
    bool p2xEq = fabs(l.p2.x - r.p2.x) < EPSILON;
    bool p2yEq = fabs(l.p2.y - r.p2.y) < EPSILON;
    
    if(!p1xEq && l.p1.x < r.p1.x) return true;
    if(!p1xEq && l.p1.x > r.p1.x) return false;
    if(!p1yEq && l.p1.y < r.p1.y) return true;
    if(!p1yEq && l.p1.y > r.p1.y) return false;
    if(!p2xEq && l.p2.x < r.p2.x) return true;
    if(!p2xEq && l.p2.x > r.p2.x) return false;
    if(!p2yEq && l.p2.y < r.p2.y) return true;
    if(!p2yEq && l.p2.y > r.p2.y) return false;
    return false;
}

VectorFileGeneratorState::VectorFileGeneratorState()
: position(ofVec2d(0, 0))
, previousPosition(ofVec2d(0, 0))
, heading(0)
, angle(90)
, edgeLength(4.0)
, filename("")
{
    uniqueSegments.reset(new SegmentSet());
}

namespace {
    
    void forward(VectorFileGeneratorState& state) {
        state.previousPosition = state.position;
        state.position += ofVec2d(cos(state.heading * DEG_TO_RAD), sin(state.heading * DEG_TO_RAD)) * state.edgeLength;
    }
    
    void forward_draw(VectorFileGeneratorState& state) {
        state.previousPosition = state.position;
        state.position += ofVec2d(cos(state.heading * DEG_TO_RAD), sin(state.heading * DEG_TO_RAD)) * state.edgeLength;

        Segment s(state.previousPosition, state.position);
        auto result = state.uniqueSegments->insert(s);
        if (result.second) {
            vectorGfx.line(state.previousPosition.x, state.previousPosition.y, state.position.x, state.position.y);
        }
    }

    void rotate_cw(VectorFileGeneratorState& state) {
        state.heading = fmod(state.heading + state.angle, 360.0);
    }

    void rotate_ccw(VectorFileGeneratorState& state) {
        state.heading = fmod(state.heading - state.angle, 360.0);
    }
    
    void start_shape(VectorFileGeneratorState& state) {
        vectorGfx.beginShape();
    }

    void end_shape(VectorFileGeneratorState& state) {
        vectorGfx.endShape();  
    }
}

VectorFileGenerator::VectorFileGenerator() {    
    add(Symbol('F').action(forward_draw));
    add(Symbol('f').action(forward));
    add(Symbol('+').action(rotate_ccw));
    add(Symbol('-').action(rotate_cw));
    add(Symbol('[').startsGroup().action(start_shape));
    add(Symbol(']').endsGroup().action(end_shape));
}

void VectorFileGenerator::begin(VectorFileGeneratorState& state) {
    vectorGfx.disableDraw();
    vectorGfx.beginEPS(state.filename);
    vectorGfx.beginShape();
}

void VectorFileGenerator::end(VectorFileGeneratorState& state) {
    ofLog() << state.uniqueSegments->size() << " unique segments";
    vectorGfx.endShape();
    vectorGfx.endEPS();
}
