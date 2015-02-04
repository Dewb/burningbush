//
//  ParagraphFormatter.h
//  burningbush
//
//  Created by Dewb on 1/6/15.
//
//

#ifndef __burningbush__ParagraphFormatter__
#define __burningbush__ParagraphFormatter__

#include <ofTrueTypeFont.h>

class ParagraphFormatter {
public:
    typedef enum {
        UpperLeft,
        LowerLeft,
        UpperRight,
        LowerRight
    } PinDirection;
    
    ParagraphFormatter(float x, float y,
                       ofTrueTypeFont* normalFont, ofTrueTypeFont* smallFont,
                        PinDirection dir = UpperLeft);
    void restart(float x, float y, PinDirection dir = UpperLeft);
    void printLine(const string& line, const string& super = "", int superPos = 0);
    void drawText(const string& text, float x, float y);
    void drawSubscript(const string& sub, float x, float y);
    float measureString(const string& str) const;
    void breakParagraph();
protected:
    ofTrueTypeFont* normalFont;
    ofTrueTypeFont* smallFont;
    PinDirection pinDirection;
    float nextLineStartX;
    float nextLineStartY;
    float lineSpacing;
};


#endif /* defined(__burningbush__ParagraphFormatter__) */
