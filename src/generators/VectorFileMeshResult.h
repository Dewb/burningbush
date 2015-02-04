//
//  VectorFileMeshResult.h
//  burningbush
//
//  Created by Dewb on 1/21/15.
//
//

#ifndef __burningbush__VectorFileMeshResult__
#define __burningbush__VectorFileMeshResult__

#include <MeshGenerator.h>

class VectorFileMeshResult : public MeshResult {
public:
    virtual void addSegment(const ofVec3f& pt1, const ofVec3f& pt2, float radius, const ofColor& color);
    virtual void addPolygon(const vector<ofVec3f>& polyPoints, const vector<ofColor>& colors);
    virtual void begin();
    virtual void end();
    string filename;
    int scale;
};

#endif /* defined(__burningbush__VectorFileMeshResult__) */
