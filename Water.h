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
    std::vector<std::vector<float>> wavegrid;

public:

    /** Constructor -- creates water layer with a given size (int type prevents difficult border sizing. */
    explicit Water(int size);

    ~Water();

    /** Animates water by one tick. */
    void update();

    void draw();


};


#endif //GRAPHICS_CW_WATER_H
