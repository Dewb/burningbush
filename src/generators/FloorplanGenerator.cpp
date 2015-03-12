//
//  FloorplanGenerator.cpp
//  burningbush
//
//  Created by Dewb on 1/22/15.
//
//

#include "FloorplanGenerator.h"
#include "ofMain.h"
//#include "ofxVectorGraphics.h"

std::ostream& operator<<(std::ostream& os, const Space& space) {
    os << "function:" << space.function << "dims: " << space.dimensions << " position: " << space.position;
    return os;
}

namespace {

    //ofxVectorGraphics vectorGfx;

    
    struct ConnectionResult {
        ConnectionResult(bool v) : connected(v) {}
        ofVec2f connectionPoint;
        ofVec2f overlapEndpoint1;
        ofVec2f overlapEndpoint2;
        bool connected;
    };
    
    ConnectionResult whereDoSpacesConnect(const Space& space1, const Space& space2, float minimumDoorWidth = 1.5) {

        // todo: check levels
        
        // Do the two spaces touch, with enough overlap to fit a door?
        float d = minimumDoorWidth;
        float x = fabs(space1.getPosition()[0] - space2.getPosition()[0]) - 0.5 * (space1.getWorldDimensions()[0] + space2.getWorldDimensions()[0]);
        float y = fabs(space1.getPosition()[1] - space2.getPosition()[1]) - 0.5 * (space1.getWorldDimensions()[1] + space2.getWorldDimensions()[1]);
        
        // If not, no overlap
        if (!(x == 0 && y < -d) &&
            !(y == 0 && x < -d))
            return ConnectionResult(false);
        
        // Where do they touch (midpoint of overlap)?
        ofVec2f connectionPoint;
        int matchedAxis = (x == 0 ? 0 : 1);
        int unmatchedAxis = (matchedAxis == 1 ? 0 : 1);
        connectionPoint[matchedAxis] = (space1.getPosition()[matchedAxis] < space2.getPosition()[matchedAxis] ? -0.5 : 0.5) *
        space2.getWorldDimensions()[matchedAxis] + space2.getPosition()[matchedAxis];
        
        // Place most doors at the centerpoint between spaces for now, but prefer placing stairwell doors directly in front of a run
        float positionOfDoor = 0.5;
//        if (space1.data.type == "Stairs") {
//            positionOfDoor = 0.75;
//        } else if (space2.data.type == "Stairs") {
//            positionOfDoor = 0.25;
//        }
        
        float overlapEnd1, overlapEnd2;
        overlapEnd1 = max(space1.getPosition()[unmatchedAxis] - space1.getWorldDimensions()[unmatchedAxis] / 2,
                          space2.getPosition()[unmatchedAxis] - space2.getWorldDimensions()[unmatchedAxis] / 2);
        overlapEnd2 = min(space1.getPosition()[unmatchedAxis] + space1.getWorldDimensions()[unmatchedAxis] / 2,
                          space2.getPosition()[unmatchedAxis] + space2.getWorldDimensions()[unmatchedAxis] / 2);
        connectionPoint[unmatchedAxis] = overlapEnd1 + (overlapEnd2 - overlapEnd1) * positionOfDoor;
        
        // for stairs only
//        if (!entersOnValidFace(space1, connectionPoint) || !entersOnValidFace(space2, connectionPoint)) {
//            return ConnectionResult(false);
//        }
        
        ofVec2f overlapPoint1(0, 0);
        ofVec2f overlapPoint2(0, 0);
        overlapPoint1[matchedAxis] = connectionPoint[matchedAxis];
        overlapPoint1[unmatchedAxis] = overlapEnd1;
        overlapPoint2[matchedAxis] = connectionPoint[matchedAxis];
        overlapPoint2[unmatchedAxis] = overlapEnd2;
        
        ConnectionResult result(true);
        result.connectionPoint = connectionPoint;
        result.overlapEndpoint1 = overlapPoint1;
        result.overlapEndpoint2 = overlapPoint2;
        return result;
    }
    
    float snapScalar(float val) {
        // Round to nearest half unit in each dimension
        return round(val * 2) / 2;
    }
    
    bool doSpacesOverlap(const Space& space1, const Space& space2) {
        return
            (fabs(space1.getPosition()[0] - space2.getPosition()[0]) * 2
                < (space1.getWorldDimensions()[0] + space2.getWorldDimensions()[0])) &&
            (fabs(space1.getPosition()[1] - space2.getPosition()[1]) * 2
                < (space1.getWorldDimensions()[1] + space2.getWorldDimensions()[1]));
    }
    
    void positionSpaceRelativeToOtherSpace(Space& space, Space& otherSpace, const ofVec2f& otherOrigin,
                                           const ofVec2f& moveVec, const ofVec2f& previousMoveVec)  {
        float minX = (otherSpace.getWorldDimensions()[0] + space.getWorldDimensions()[0]) / 2;
        float minY = (otherSpace.getWorldDimensions()[1] + space.getWorldDimensions()[1]) / 2;
        float tx = fabs(minX / moveVec[0]);
        float ty = fabs(minY / moveVec[1]);
        float dx = moveVec[0] * min(tx, ty);
        float dy = moveVec[1] * min(tx, ty);
        
        space.setPosition(otherOrigin[0] + dx, otherOrigin[1] + dy);
       
        if ((otherSpace.function == "Hall" && space.function == "Hall") &&
            (otherSpace.getWorldDimensions()[0] == space.getWorldDimensions()[1] ||
             otherSpace.getWorldDimensions()[1] == space.getWorldDimensions()[0])) {
                
            if (fabs(moveVec[0]) > fabs(moveVec[1])) {
                dy += 0.5 * (otherSpace.getWorldDimensions()[1] - space.getWorldDimensions()[1])
                        * (previousMoveVec[1] < 0 ? -1 : 1);
            } else {
                dx += 0.5 * (otherSpace.getWorldDimensions()[0] - space.getWorldDimensions()[0])
                        * (previousMoveVec[0] < 0 ? -1 : 1);
            }

            space.setPosition(otherSpace.getPosition()[0] + dx,
                              otherSpace.getPosition()[1] + dy);
        }
        
        if (doSpacesOverlap(space, otherSpace)) {
            ofLog() << "Placement failed to prevent overlap";
            ofLog() << "space " << space;
            ofLog() << "other " << otherSpace;
            ofLog() << "(" << moveVec << ") dx: " << dx << " dy: " << dy;
        }
    }

    bool layoutContainsOverlaps(ofPtr<vector<Space> > spaces) {
/*
        for (int ii = 0; ii < spaces->size() - 1; ii++) {
            for (int jj = ii + 1; jj < spaces->size(); jj++) {
                if (doSpacesOverlap(spaces->at(ii), spaces->at(jj))) {
                    return true;
                }
            }
        }
*/
        return false;
    }
    
    void addSpace(FloorplanGeneratorState& state, Space& newSpace) {
        newSpace.setPosition(state.position.x, state.position.y);
        if (state.results) {
            
            if (state.results->size()) {
                ofVec2f headingVec(cos(state.heading * DEG_TO_RAD), sin(state.heading * DEG_TO_RAD));
                ofVec2f previousHeadingVec(cos(state.previousHeading * DEG_TO_RAD), sin(state.previousHeading * DEG_TO_RAD));

                state.results->push_back(newSpace);
                Space& space = state.results->back();
                Space& lastSpace = state.results->at(state.lastSpaceIndex);
                ofVec2f lastOrigin = lastSpace.position;
                int tries = 10;
                while (tries--) {
                    positionSpaceRelativeToOtherSpace(space, lastSpace, lastOrigin,
                                                      headingVec, previousHeadingVec);
                    if (layoutContainsOverlaps(state.results)) {
                        ofVec2f nudgeVec = previousHeadingVec.normalized();
                        auto newPos = lastSpace.getPosition() + 0.5 * nudgeVec;
                        auto newDim = lastSpace.getWorldDimensions() + 1.0 * nudgeVec;
                        lastOrigin += 1.0 * nudgeVec;
                        lastSpace.position = newPos;
                        lastSpace.dimensions = newDim;
                    } else {
                        break;
                    }
                }
                newSpace = space;
            } else {
                state.results->push_back(newSpace);
            }

            state.lastSpaceIndex = state.results->size() - 1;
        }

        ofPushStyle();
        ofSetColor(ofColor::red);
        ofLine(state.position.x, state.position.y, newSpace.position.x, newSpace.position.y);
        ofPopStyle();
        
        state.position = newSpace.position;
    }
    
    void place_entry(FloorplanGeneratorState& state, FloatParams& params) {
        Space space;
        space.function = "Entry";
        space.dimensions.x = 12.5;
        space.dimensions.y = 5;
        space.isCirculation = true;
        addSpace(state, space);
    }

    void place_horizontal_circulation(FloorplanGeneratorState& state, FloatParams& params) {
        float length = 5;

        Space space;
        space.function = "Hall";
        ofVec2f headingVec(cos(state.heading * DEG_TO_RAD), sin(state.heading * DEG_TO_RAD));
        ofVec2f headingNormal(headingVec.y, -headingVec.x);
        space.dimensions.x = fabs(headingVec[0] * length) + fabs(headingNormal[0] * state.circulationWidth);
        space.dimensions.y = fabs(headingVec[1] * length) + fabs(headingNormal[1] * state.circulationWidth);
        space.isCirculation = true;
        
        addSpace(state, space);
    }

    void place_vertical_circulation(FloorplanGeneratorState& state, FloatParams& params) {
    
    }

    void place_functional_space(FloorplanGeneratorState& state, FloatParams& params) {
        Space space;
        space.function = "Occupiable";
        space.dimensions.x = 5;
        space.dimensions.y = 5;
        space.isCirculation = false;
        addSpace(state, space);
    }
    
    void place_services(FloorplanGeneratorState& state, FloatParams& params) {
        
    }
    
    void rotate_cw(FloorplanGeneratorState& state, FloatParams& params) {
        state.previousHeading = state.heading;
        state.heading = fmod(state.heading + state.angle, 360.0f);
    }
    
    void rotate_ccw(FloorplanGeneratorState& state, FloatParams& params) {
        state.previousHeading = state.heading;
        state.heading = fmod(state.heading - state.angle, 360.0f);
    }
    
}

FloorplanGeneratorState::FloorplanGeneratorState()
: angle(90)
, heading(0)
, position(0, 0)
, lastSpaceIndex(0)
, circulationWidth(2.5)
{
}

FloorplanGenerator::FloorplanGenerator() {
    add(Symbol('E').action(place_entry));
    add(Symbol('C').action(place_horizontal_circulation));
    add(Symbol('V').action(place_vertical_circulation));
    add(Symbol('F').action(place_functional_space));
    add(Symbol('S').action(place_services));
    add(Symbol('+').action(rotate_ccw));
    add(Symbol('-').action(rotate_cw));
    add(Symbol('[').startsGroup());
    add(Symbol(']').endsGroup());
}

void FloorplanGenerator::begin(FloorplanGeneratorState& state) {
    
}

void FloorplanGenerator::end(FloorplanGeneratorState& state) {
    for (auto& space : *state.results) {
        ofNoFill();
        ofRect(space.position.x - space.dimensions.x / 2.0,
                       space.position.y - space.dimensions.y / 2.0,
                       space.dimensions.x,
                       space.dimensions.y);
        ofLog() << space;
    }
}


