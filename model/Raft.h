//
// Created by Edward Gargan on 21/04/2018.
//

#ifndef GRAPHICS_CW_RAFT_H
#define GRAPHICS_CW_RAFT_H

#include "FloatingModel.h"
#include "Lantern.h"


class Raft : public FloatingModel {

    /** Pointer to raft's lantern object. */ // Separated due to complexity of lantern model.
    Lantern* lantern;

    /** Height of raft's mast. */
    const float mastHeight;

    /** Radius of cylindrical 'logs' that make up raft floor. */
    float baseLogRadius;

    /** Size of gap between logs. */
    float baseLogGap;

    /** Number of length-way floor logs. */
    int numBaseLogs;

    // TODO: move elsewhere? 'Raft' doesn't really need to store these + maybe set nicer colours!
    /** Array of 'nice' brown colours, matching box textures. */
    std::vector<Vec3f> browns = {Vec3f{25, 17, 9}/255, Vec3f{20, 12, 8}/255,
                                 Vec3f{20, 12, 0}/255, Vec3f{28, 19, 16}/255,
                                 Vec3f{30, 21, 9}/255};

    /** Array of 'browns' of colours that will colour each 'log' cylinder in the raft's base. */
    int* baseLogColours;

    // Private draw methods for component shapes.
    void drawLog(float radius, float length, Vec3f colr) const;
    void drawSail() const;
    void drawLamp() const;



public:

    Raft(Water* water, Vec3f _location, float _width, float _length, float height);

    ~Raft();

    void draw() const;

    void update();

};


#endif //GRAPHICS_CW_RAFT_H
