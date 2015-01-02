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
        return ofColor(220, 210, 180);
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

    void addTriangleToMesh(ofPtr<ofMesh> mesh, const ofVec3f& a, const ofVec3f& b, const ofVec3f& c, const ofColor& color) {
        unsigned start = mesh->getVertices().size();
        mesh->addIndex(start + 0);
        mesh->addIndex(start + 1);
        mesh->addIndex(start + 2);
        mesh->addVertex(a);
        mesh->addVertex(b);
        mesh->addVertex(c);
        mesh->addColor(color);
        mesh->addColor(color);
        mesh->addColor(color);
    }
    
    void addPolygonToMesh(ofPtr<ofMesh> mesh, const vector<ofVec3f> points, const ofColor& color) {
        for (int i = 1; i < points.size() - 1; i++) {
            addTriangleToMesh(mesh, points[0], points[i], points[i + 1], color);
        }
    }
    
}

// These should be anon lambdas when oF supports C++11
namespace {
    
    void forward(MeshGeneratorState& state) {
        state.position += state.heading.normalized() * state.segmentLength;
        state.pointHistory.push_back(state.position);
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
        state.segmentRadius *= 0.75;
    }
    
    void next_color(MeshGeneratorState& state) {
        state.currentColor++;
    }
    
    void next_color_series(MeshGeneratorState& state) {
        state.currentColorSeries++;
    }

    void begin_polygon(MeshGeneratorState& state) {
        // history should already be clear because it isn't copied, but just in case
        state.pointHistory.clear();
        state.pointHistory.push_back(state.position);
    }

    void end_polygon(MeshGeneratorState& state) {
        addPolygonToMesh(state.mesh, state.pointHistory, state.getCurrentColor());
    }

}

MeshGenerator::MeshGenerator() {
    add(Token('F').action(forward_draw));
    add(Token('f').action(forward));
    add(Token('+').action(turn_left));
    add(Token('-').action(turn_right));
    add(Token('&').action(pitch_down));
    add(Token('^').action(pitch_up));
    add(Token('\\').action(roll_left));
    add(Token('/').action(roll_right));
    add(Token('|').action(turn_around));
    add(Token('!').action(decrease_diameter));
    add(Token('\'').action(next_color));
    add(Token('"').action(next_color_series));
    add(Token('[').startsGroup());
    add(Token(']').endsGroup());
    add(Token('{').startsGroup().action(begin_polygon));
    add(Token('}').endsGroup().action(end_polygon));
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
