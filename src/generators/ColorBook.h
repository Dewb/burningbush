//
//  ColorBook.h
//  burningbush
//
//  Created by Michael Dewberry on 1/30/15.
//
//

#ifndef __burningbush__ColorBook__
#define __burningbush__ColorBook__

#include "ofTypes.h"

class ColorBook {
public:
    ColorBook();

    const ofColor& getColor(int series, int index);
    
    void add(const ofColor& c, int number = 1);
    void addGradient(const ofColor& c1, const ofColor& c2, int steps = 3);
    void addGradient(const ofColor& next, int steps = 3);
    void nextSeries();
    
    int getNumSeries() const;
    int getNumColors(int series) const;

protected:
    vector<vector<ofColor> > colors;
};


#endif /* defined(__burningbush__ColorBook__) */
