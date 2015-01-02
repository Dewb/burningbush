//
//  MeshGenerator.cpp
//  burningbush
//
//  Created by Dewb on 12/28/14.
//
//

#include "MeshGenerator.h"
#include <ofMatrix4x4.h>

MeshGeneratorState::MeshGeneratorState(const MeshGeneratorState& other) {
    position = other.position;
    heading = other.heading;
    up = other.up;
    left = other.left;
    angle = other.angle;
    segmentLength = other.segmentLength;
    segmentRadius = other.segmentRadius;
    mesh = other.mesh;
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
    }
    
    void addSegmentToMesh(ofPtr<ofMesh> mesh, const ofVec3f& pt1, const ofVec3f& pt2, float radius) {
        float length = (pt1 - pt2).length();
        ofMesh segmentMesh = ofMesh::cylinder(radius, length, 8, 1, 2, true, OF_PRIMITIVE_TRIANGLES);

        ofMatrix4x4 shiftOrigin;
        shiftOrigin.makeTranslationMatrix(ofVec3f(0, length * 0.5, 0));
        ofMatrix4x4 orient;
        orient.makeRotationMatrix(ofVec3f(0, 1, 0), pt2 - pt1);
        orient.translate(pt1);
        mergeIntoMesh(*(mesh.get()), segmentMesh, shiftOrigin * orient);
    }

    void addTriangleToMesh(ofPtr<ofMesh> mesh, const ofVec3f& a, const ofVec3f& b, const ofVec3f& c) {
        unsigned start = mesh->getVertices().size();
        mesh->addIndex(start + 0);
        mesh->addIndex(start + 1);
        mesh->addIndex(start + 2);
        mesh->addVertex(a);
        mesh->addVertex(b);
        mesh->addVertex(c);
    }
    
    void addPolygonToMesh(ofPtr<ofMesh> mesh, const vector<ofVec3f> points) {
        for (int i = 1; i < points.size() - 1; i++) {
            addTriangleToMesh(mesh, points[0], points[i], points[i + 1]);
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
        addSegmentToMesh(state.mesh, state.position, previous, state.segmentRadius);
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
        state.left.rotate(180, state.heading);
        state.up.rotate(180, state.heading);
    }
    
    void decrease_diameter(MeshGeneratorState& state) {
        state.segmentRadius *= 0.75;
    }
    
    void change_color(MeshGeneratorState& state) {
    }

    void begin_polygon(MeshGeneratorState& state) {
        // history should already be clear because it isn't copied, but just in case
        state.pointHistory.clear();
        state.pointHistory.push_back(state.position);
    }

    void end_polygon(MeshGeneratorState& state) {
        //addPolygonToMesh(state.mesh, state.pointHistory);
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
    add(Token('\'').action(change_color));
    add(Token('[').startsGroup());
    add(Token(']').endsGroup());
    add(Token('{').startsGroup().action(begin_polygon));
    add(Token('}').endsGroup().action(end_polygon));
}

void MeshGenerator::begin(MeshGeneratorState& state) {
    state.mesh.reset(new ofMesh());
    state.mesh->setMode(OF_PRIMITIVE_TRIANGLES);
    state.mesh->enableIndices();
}

void MeshGenerator::end(MeshGeneratorState& state) {
    //state.mesh->mergeDuplicateVertices();
}
