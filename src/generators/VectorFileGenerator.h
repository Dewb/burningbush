//
//  VectorFileGenerator.h
//  burningbush
//
//  Created by Dewb on 12/28/14.
//
//

#ifndef __burningbush__VectorFileGenerator__
#define __burningbush__VectorFileGenerator__

#include "LSystemGenerator.h"
#include "ofTypes.h"
#include <set>

class ofVec2d {
public:
    double x;
    double y;
    ofVec2d(double _x = 0, double _y = 0) : x(_x), y(_y) {}
    inline ofVec2d operator*(double d) const {
        return ofVec2d(x * d, y * d);
    }
    inline ofVec2d operator=(const ofVec2f& l) const {
        return ofVec2d(l.x, l.y);
    }
    inline ofVec2d normalized() const {
        double length = (double)sqrt(x * x + y * y);
        if( length > 0 ) {
            return ofVec2d(x / length, y / length);
        } else {
            return ofVec2d();
        }
    }
    inline ofVec2d& rotate(double angle) {
        double a = (double)(angle * DEG_TO_RAD);
        double xrot = x * cos(a) - y * sin(a);
        y = x * sin(a) + y * cos(a);
        x = xrot;
        return *this;
    }
    inline ofVec2d& operator+=(const ofVec2d& vec) {
        x += vec.x;
        y += vec.y;
        return *this;
    }
    inline double distance(const ofVec2d& pnt) const {
        float vx = x - pnt.x;
        float vy = y - pnt.y;
        return (double)sqrt(vx * vx + vy * vy);
    }
};

struct Segment {
    ofVec2d p1;
    ofVec2d p2;
    Segment(const ofVec2d& l, const ofVec2d& r);
};
std::ostream& operator<<(std::ostream& os, const Segment& seg);

struct SegmentCompare {
    bool operator()(const Segment& l, const Segment& r) const;
};

typedef set<Segment, SegmentCompare> SegmentSet;

class VectorFileGeneratorState {
public:
    VectorFileGeneratorState();
    
    ofVec2d position;
    ofVec2d previousPosition;
    double heading;
    float angle;
    float edgeLength;

    string filename;
    ofPtr<SegmentSet> uniqueSegments;
};

class VectorFileGenerator : public Generator<VectorFileGeneratorState> {
public:
    VectorFileGenerator();
    virtual void begin(VectorFileGeneratorState& state);
    virtual void end(VectorFileGeneratorState& state);
};

#endif /* defined(__burningbush__VectorFileGenerator__) */
