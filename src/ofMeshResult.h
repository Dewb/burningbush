//
//  ofMeshResult.h
//  burningbush
//
//  Created by Dewb on 1/21/15.
//
//

#ifndef __burningbush__ofMeshResult__
#define __burningbush__ofMeshResult__

#include <ofMesh.h>
#include <MeshGenerator.h>

class ofMeshResult : public MeshResult {
public:
    virtual void addSegment(const ofVec3f& pt1, const ofVec3f& pt2, float radius, const ofColor& color);
    virtual void addPolygon(const vector<ofVec3f>& polyPoints, const vector<ofColor>& colors);
    virtual void begin();
    virtual void end();
    ofPtr<ofMesh> mesh;
};

#endif /* defined(__burningbush__ofMeshResult__) */
