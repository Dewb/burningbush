//
//  VectorFileMeshResult.cpp
//  burningbush
//
//  Created by Dewb on 1/21/15.
//
//

#include "VectorFileMeshResult.h"
#include "ofxVectorGraphics.h"

namespace {
    ofxVectorGraphics vectorGfx;
}

void VectorFileMeshResult::addSegment(const ofVec3f& pt1, const ofVec3f& pt2, float radius, const ofColor& color) {
    vectorGfx.setColor(color.getHex());
    vectorGfx.setLineWidth(radius * 2.0);
    vectorGfx.line(pt1.x, pt1.y, pt2.x, pt2.y);
}

void VectorFileMeshResult::addPolygon(const vector<ofVec3f>& polyPoints, const vector<ofColor>& colors) {
    vectorGfx.beginShape();
    vectorGfx.fill();
    vectorGfx.setColor(colors[0].getHex());
    
    auto points = polyPoints;
    if (points.back() != points.front()) {
        points.push_back(points.front());
    }
    for (int i = 1; i < points.size() - 1; i++) {
        vectorGfx.polyVertex(points[i].x, points[i].y);
    }
    
    vectorGfx.endShape();
}

void VectorFileMeshResult::begin() {
    vectorGfx.disableDraw();
    vectorGfx.beginEPS(filename, -scale/2.0, scale/2.0, scale, scale);
}

void VectorFileMeshResult::end() {
    vectorGfx.endEPS();
}
