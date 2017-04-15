//
//  SyphonTextRenderer.cpp
//  burningbush
//
//  Created by Michael Dewberry on 3/14/15.
//
//

#include "SyphonTextRenderer.h"
#include "ParagraphFormatter.h"

SyphonTextRenderer::SyphonTextRenderer() {
    initialized = false;
    visible = true;
}

void SyphonTextRenderer::initialize(LSystem* sys, int iterationCount,
                                    std::string serverName, int width, int height,
                                    std::string fontName, int fontSize, int _margin) {
    system = sys;
    iterations = iterationCount;
    margin = _margin;

    syphonServer.setName(serverName);
    fbo.allocate(width, height, GL_RGBA);

    font.loadFont(fontName, fontSize);
    
    srand(time(0));
    newText();

    initialized = true;
}

SyphonTextRenderer::~SyphonTextRenderer() {
}

void SyphonTextRenderer::newText() {
    system->reseed(rand());
    current = to_string(system->generate(iterations));
}

bool SyphonTextRenderer::isVisible() {
    return visible;
}

void SyphonTextRenderer::hide() {
    visible = false;
}

void SyphonTextRenderer::show() {
    visible = true;
}

void SyphonTextRenderer::update() {
    if (!initialized & !visible) {
        return;
    }

    fbo.begin();
    ofPushStyle();
    ofDisableDepthTest();

    ofBackground(0, 0, 0, 0);

    ofSetColor(80, 80, 40);
    ParagraphFormatter shadow(margin + 2, margin + 2, &font, NULL);
    shadow.printLine(current);

    ofSetColor(255, 255, 210);
    ParagraphFormatter para(margin, margin, &font, NULL);
    para.printLine(current);

    ofEnableDepthTest();
    ofPopStyle();
    fbo.end();

    syphonServer.publishTexture(&(fbo.getTextureReference()));
}

void SyphonTextRenderer::draw(float x, float y) {
    if (visible) {
        fbo.draw(x, y);
    }
}