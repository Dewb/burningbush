//
//  ColorBook.cpp
//  burningbush
//
//  Created by Michael Dewberry on 1/30/15.
//
//

#include "ColorBook.h"

namespace {
    ofColor hsbLerp(const ofColor& c1, const ofColor& c2, float s) {
        float h1, s1, b1, h2, s2, b2;
        c1.getHsb(h1, s1, b1);
        c2.getHsb(h2, s2, b2);
        ofColor r;
        r.setHsb(h1 + s * (h2 - h1),
                 s1 + s * (s2 - s1),
                 b1 + s * (b2 - b1));
        return r;
    }
}

ColorBook::ColorBook() {
    colors.push_back(vector<ofColor>());
}

const ofColor& ColorBook::getColor(int series, int index) {
    if (series < colors.size() && series >= 0 &&
        index < colors[series].size() && index >= 0) {
        return colors[series][index];
    } else {
        return ofColor::black;
    }
}

void ColorBook::add(const ofColor& c, int number) {
    for (int i = 0; i < number; i++) {
        colors.back().push_back(c);
    }
}

void ColorBook::addGradient(const ofColor& c1, const ofColor& c2, int number) {
    for (int i = 0; i < number; i++) {
        add(hsbLerp(c1, c2, i / (number - 1.0)));
    }
}

void ColorBook::addGradient(const ofColor& next, int number) {
    if (colors.back().size() > 0) {
        addGradient(colors.back().back(), next, number);
    } else {
        add(next, number);
    }
}

void ColorBook::nextSeries() {
    colors.push_back(vector<ofColor>());
}

int ColorBook::getNumSeries() const {
    return colors.size();
}

int ColorBook::getNumColors(int series) const {
    if (series < colors.size() && series >= 0) {
        return colors[series].size();
    } else {
        return 0;
    }
}
