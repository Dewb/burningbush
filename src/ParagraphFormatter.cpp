//
//  ParagraphFormatter.cpp
//  burningbush
//
//  Created by Dewb on 1/6/15.
//
//

#include "ParagraphFormatter.h"
#include <ofMain.h>

ParagraphFormatter::ParagraphFormatter(float x, float y,
                                                 ofTrueTypeFont* normal,
                                                 ofTrueTypeFont* small,
                                                 PinDirection dir) {
    nextLineStartX = x;
    nextLineStartY = y;
    pinDirection = dir;
    
    normalFont = normal;
    smallFont = small;
    
    if (normalFont) {
        lineSpacing = normalFont->getLineHeight() + 2.0;
    } else {
        lineSpacing = 18.0;
    }
    if (pinDirection == UpperLeft || pinDirection == UpperRight) {
        nextLineStartY += lineSpacing * 0.5;
    } else if (pinDirection == LowerLeft || pinDirection == LowerRight) {
        nextLineStartY -= lineSpacing * 0.5;
    }
}

void ParagraphFormatter::breakParagraph() {
    if (pinDirection == UpperLeft || pinDirection == UpperRight) {
        nextLineStartY += lineSpacing * 0.5;
    } else if (pinDirection == LowerLeft || pinDirection == LowerRight) {
        nextLineStartY -= lineSpacing * 0.5;
    }
}

void ParagraphFormatter::restart(float x, float y, PinDirection dir) {
    nextLineStartX = x;
    nextLineStartY = y;
    pinDirection = dir;
}

float ParagraphFormatter::measureString(const string& str) const {
    if (normalFont) {
        auto rect = normalFont->getStringBoundingBox(str, 0, 0);
        return rect.width;
    } else {
        return str.length() * 8.0;
    }
}

void ParagraphFormatter::printLine(const string& line, const string& super, int superPos) {
    string formattedLine = line;
    int pos = 0;
    while ((pos = formattedLine.find("_", pos)) != formattedLine.npos && pos < line.length() - 1) {
        float x = nextLineStartX + measureString(formattedLine.substr(0, pos));
        string sub = " ";
        sub[0] = formattedLine[pos + 1];
        drawSubscript(sub, x, nextLineStartY + lineSpacing * 0.25);
        formattedLine.replace(pos, 2, " ");
    }
    
    if (super.length()) {
        float x = nextLineStartX + measureString(formattedLine.substr(0, superPos));
        drawSubscript(super, x, nextLineStartY - lineSpacing * 0.4);
    }
    
    float x = nextLineStartX;
    if (pinDirection == UpperRight || pinDirection == LowerRight) {
        x -= measureString(formattedLine);;
    }
    
    drawText(formattedLine, x, nextLineStartY);
    
    if (pinDirection == UpperLeft || pinDirection == UpperRight) {
        nextLineStartY += lineSpacing;
    } else if (pinDirection == LowerLeft || pinDirection == LowerRight) {
        nextLineStartY -= lineSpacing;
    }
}

void ParagraphFormatter::drawText(const string& text, float x, float y) {
    if (normalFont) {
        normalFont->drawString(text, x, y);
    } else {
        ofDrawBitmapString(text, x, y);
    }
}

void ParagraphFormatter::drawSubscript(const string& sub, float x, float y) {
    if (smallFont) {
        smallFont->drawString(sub, x, y);
    } else {
        ofDrawBitmapString(sub, x, y);
    }
}
