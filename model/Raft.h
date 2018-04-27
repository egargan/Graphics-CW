//
// Created by Edward Gargan on 21/04/2018.
//

#ifndef GRAPHICS_CW_RAFT_H
#define GRAPHICS_CW_RAFT_H

#include "FloatingModel.h"
#include "Lantern.h"

// TODO: write
/**
 *
 */
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

    /** List of random numbers used to add random variation to e.g. log orientation and position.
     *  Populated on construction. */
    float* varylist;

    /** Size of varylist. */
    const int varylistsize;

    /** Pointer to list of texture IDs for a number of log textures. */
    int* logTexIds;

    /** Texture ID of 'end' log texture / cross-section texture. */
    int logEndTexId;

    /** Draws log of given dimensions. */
    void drawLog(float radius, float length, int) const;


public:

    /** Constructs raft model with given dimensions and pointer to the Water model it 'sits' on. */
    Raft(Water* water, Vec3f _location, float _width, float _length, float height);

    ~Raft();

    void draw() const;
    void update();

};


#endif //GRAPHICS_CW_RAFT_H
