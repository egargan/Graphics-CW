//
// Created by Edward Gargan on 10/04/2018.
//

#ifndef GRAPHICS_CW_WATER_H
#define GRAPHICS_CW_WATER_H


#include "Model.h"

#include <vector>
#include <cmath>

class Water : Model {

    /** Holds y-displacement for each point in water, describing current 'wave' shape of water layer. */
    std::vector<std::vector<float>> mesh;

    /** Side-length of individual mesh tile. */
    float tilelength;

    /** Time counter for water flow. */
    float time = 0.f;

    /** Change in time counter per update(). */
    const float dtime = 0.002f;

    /** Amplitude, i.e. height, of water waves. */
    const float amplitude = 0.6f;


public:

    /** Constructor -- creates water mesh of 'n' unit tiles. */
    explicit Water(int n);

    /** Constructor -- creates water mesh of 'n' tiles of given size. */
    Water(float, int n);

    ~Water();

    /** Animates water by one tick. */
    void update();

    void draw() const;


    /** Returns a surface normal of the triangle described by 3 points on the water's surface. */
    Vec3f getSurfaceNormal(int, int, int, int, int, int) const;

    /** Returns Y-displacement at given mesh coordinates. */
    float getMeshY(int x, int z) const;


    float getTileLength() const { return tilelength; }

    int getMeshLength() const { return (int) mesh.size(); }

};


#endif //GRAPHICS_CW_WATER_H
