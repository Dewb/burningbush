//
//  ofMeshResult.cpp
//  burningbush
//
//  Created by Dewb on 1/21/15.
//
//

#include "ofMeshResult.h"
#include "ofMatrix4x4.h"

namespace {
    
    void mergeIntoMesh(ofPtr<ofMesh> target, const ofMesh& newMesh, const ofMatrix4x4& transform) {
        int startIndex = target->getVertices().size();
        for (auto& index : newMesh.getIndices()) {
            target->addIndex(index + startIndex);
        }
        for (auto& vertex : newMesh.getVertices()) {
            target->addVertex(vertex * transform);
        }
        target->addColors(newMesh.getColors());
        target->addNormals(newMesh.getNormals());
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

        ofVec3f v1, v2, n;
        v1 = a - b;
        v2 = a - c;
        n = v1.cross(v2);

        mesh->addNormal(n);
        mesh->addNormal(n);
        mesh->addNormal(n);
    }
}

void ofMeshResult::addSegment(const ofVec3f& pt1, const ofVec3f& pt2, float radius, const ofColor& color) {
    float length = (pt1 - pt2).length();
    ofMesh segmentMesh = ofMesh::cylinder(radius, length, 6, 1, 2, true, OF_PRIMITIVE_TRIANGLES);
    ofFloatColor floatColor(color);
    vector<ofFloatColor> colors(segmentMesh.getNumVertices(), floatColor);
    segmentMesh.addColors(colors);

    ofMatrix4x4 shiftOrigin;
    shiftOrigin.makeTranslationMatrix(ofVec3f(0, length * 0.5, 0));
    ofMatrix4x4 orient;
    orient.makeRotationMatrix(ofVec3f(0, 1, 0), pt2 - pt1);
    orient.translate(pt1);
    mergeIntoMesh(mesh, segmentMesh, shiftOrigin * orient);
}

void ofMeshResult::addPolygon(const vector<ofVec3f>& polyPoints, const vector<ofColor>& colors) {
    auto points = polyPoints;
    if (points.back() != points.front()) {
        points.push_back(points.front());
    }
    for (int i = 1; i < points.size() - 1; i++) {
        addTriangleToMesh(mesh, points[0], points[i], points[i + 1],
                          colors[0], colors[i], colors[i + 1]);
    }
}

void ofMeshResult::begin() {
    mesh.reset(new ofMesh());
    mesh->setMode(OF_PRIMITIVE_TRIANGLES);
    mesh->enableIndices();
    mesh->enableColors();
}

void ofMeshResult::end() {
    //mesh->mergeDuplicateVertices();
}

