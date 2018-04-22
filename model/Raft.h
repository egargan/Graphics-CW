//
// Created by Edward Gargan on 21/04/2018.
//

#ifndef GRAPHICS_CW_RAFT_H
#define GRAPHICS_CW_RAFT_H

#include "FloatingModel.h"


class Raft : public FloatingModel {

    /** Height of raft's mast. */
    const float mastHeight;

    /** Radius of cylindrical 'logs' that make up raft floor. */
    float baseLogRadius;

    /** Number of length-way floor logs. */
    int numBaseLogs;

    // Private draw methods for component shapes.
    void drawCylinder(float radius, float length) const;
    void drawSail() const;
    void drawLamp() const;

public:

    Raft(Water* water, Vec3f _location, float _width, float _length, float height);

    void draw() const;

};


#endif //GRAPHICS_CW_RAFT_H
