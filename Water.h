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
    float dtime = 0.0009f;

public:

    /** Constructor -- creates water mesh of 'n' unit tiles. */
    explicit Water(int n);

    /** Constructor -- creates water mesh of 'n' tiles of given size. */
    Water(float, int n);

    ~Water();

    /** Animates water by one tick. */
    void update();

    void draw();


};


#endif //GRAPHICS_CW_WATER_H
