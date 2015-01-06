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

MeshGeneratorState::MeshGeneratorState()
: heading(ofVec3f(0, 1, 0))
, up(ofVec3f(0, 0, 1))
, left(ofVec3f(1, 0, 0))
, currentColor(0)
, currentColorSeries(0)
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
    mesh = other.mesh;
}

ofColor MeshGeneratorState::getCurrentColor() {
    if (colorBook) {
        return colorBook->getColor(currentColorSeries, currentColor);
    } else {
        return ofColor(128, 128, 128);
    }
}

namespace {
    
    void mergeIntoMesh(ofMesh& target, const ofMesh& newMesh, const ofMatrix4x4& transform) {
        int startIndex = target.getVertices().size();
        for (auto& index : newMesh.getIndices()) {
            target.addIndex(index + startIndex);
        }
        for (auto& v : newMesh.getVertices()) {
            target.addVertex(v * transform);
        }
        for (auto& c : newMesh.getColors()) {
            target.addColor(c);
        }
    }
    
    void addSegmentToMesh(ofPtr<ofMesh> mesh, const ofVec3f& pt1, const ofVec3f& pt2, float radius, const ofColor& color) {
        float length = (pt1 - pt2).length();
        ofMesh segmentMesh = ofMesh::cylinder(radius, length, 8, 1, 2, true, OF_PRIMITIVE_TRIANGLES);
        for (auto& v : segmentMesh.getVertices()) {
            segmentMesh.addColor(color);
        }

        ofMatrix4x4 shiftOrigin;
        shiftOrigin.makeTranslationMatrix(ofVec3f(0, length * 0.5, 0));
        ofMatrix4x4 orient;
        orient.makeRotationMatrix(ofVec3f(0, 1, 0), pt2 - pt1);
        orient.translate(pt1);
        mergeIntoMesh(*(mesh.get()), segmentMesh, shiftOrigin * orient);
    }

    void addTriangleToMesh(ofPtr<ofMesh> mesh, const ofVec3f& a, const ofVec3f& b, const ofVec3f& c,
                           const ofColor& color1, const ofColor& color2, const ofColor& color3) {
        unsigned start = mesh->getVertices().size();
        mesh->addIndex(start + 0);
        mesh->addIndex(start + 1);
        mesh->addIndex(start + 2);
        mesh->addVertex(a);
        mesh->addVertex(b);
        mesh->addVertex(c);
        mesh->addColor(color1);
        mesh->addColor(color2);
        mesh->addColor(color3);
    }
    
    void addPolygonToMesh(ofPtr<ofMesh> mesh, const vector<ofVec3f>& polyPoints, const vector<ofColor>& colors) {
        auto points = polyPoints;
        if (points.back() != points.front()) {
            points.push_back(points.front());
        }
        for (int i = 1; i < points.size() - 1; i++) {
            addTriangleToMesh(mesh, points[0], points[i], points[i + 1],
                                    colors[0], colors[i], colors[i + 1]);
        }
    }
    
}

namespace {
    
    void forward(MeshGeneratorState& state) {
        state.position += state.heading.normalized() * state.segmentLength;
        state.pointHistory.push_back(state.position);
        state.colorHistory.push_back(state.getCurrentColor());
    }

    void forward_draw(MeshGeneratorState& state) {
        ofVec3f previous = state.position;
        forward(state);        
        addSegmentToMesh(state.mesh, state.position, previous, state.segmentRadius, state.getCurrentColor());
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
        state.segmentRadius *= 0.618;
    }
    
    void next_color(MeshGeneratorState& state) {
        state.currentColor++;
    }
    
    void previous_color(MeshGeneratorState& state) {
        state.currentColor--;
    }
    
    void next_color_series(MeshGeneratorState& state) {
        state.currentColorSeries++;
    }

    void begin_polygon(MeshGeneratorState& state) {
        // history should already be clear because it isn't copied, but just in case
        state.pointHistory.clear();
        state.colorHistory.clear();
        state.pointHistory.push_back(state.position);
        state.colorHistory.push_back(state.getCurrentColor());
    }

    void end_polygon(MeshGeneratorState& state) {
        addPolygonToMesh(state.mesh, state.pointHistory, state.colorHistory);
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
    add(Symbol('\'').action(next_color));
    add(Symbol('`').action(previous_color));
    add(Symbol('"').action(next_color_series));
    add(Symbol('[').startsGroup());
    add(Symbol(']').endsGroup());
    add(Symbol('{').startsGroup().action(begin_polygon));
    add(Symbol('}').endsGroup().action(end_polygon));
}

void MeshGenerator::begin(MeshGeneratorState& state) {
    state.mesh.reset(new ofMesh());
    state.mesh->setMode(OF_PRIMITIVE_TRIANGLES);
    state.mesh->enableIndices();
    state.mesh->enableColors();
}

void MeshGenerator::end(MeshGeneratorState& state) {
    //state.mesh->mergeDuplicateVertices();
}
