//
//  SyphonTextRenderer.h
//  burningbush
//
//  Created by Michael Dewberry on 3/14/15.
//
//

#ifndef __burningbush__SyphonTextRenderer__
#define __burningbush__SyphonTextRenderer__

#include <string>
#include "LSystem.h"
#include "ofTrueTypeFont.h"
#include "ofxSyphonServer.h"

class SyphonTextRenderer {
public:
    SyphonTextRenderer();
    ~SyphonTextRenderer();

    void initialize(LSystem* sys, int iterationCount,
                    std::string serverName, int width, int height,
                    std::string fontName, int fontSize, int margin);

    void newText();
    bool isVisible();
    void hide();
    void show();

    void update();
    void draw(float x, float y);

    LSystem* system;
    int iterations;

    std::string current;
    ofTrueTypeFont font;
    int margin;

protected:
    ofxSyphonServer syphonServer;
    ofFbo fbo;

    bool initialized;
    bool visible;
};

#endif /* defined(__burningbush__SyphonTextRenderer__) */
