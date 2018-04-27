//
// Created by Edward Gargan on 26/04/2018.
//

#ifndef GRAPHICS_CW_LANTERN_H
#define GRAPHICS_CW_LANTERN_H


#include "Model.h"

class Lantern : Model {

    /** Pre-computed series of 'smooth' random numbers that control light source's + candle model's flicker. */
    std::vector<float> lightVary{-0.04f, -0.04f, -0.02f, -0.01f, -0.01f, 0.01f, 0.02f, 0.04f, 0.04f, 0.03f, 0.04f, 0.05f, 0.05f, 0.03f,
                                 0.05f, 0.03f, 0.02f, 0.01f, 0.01f, -0.02f, 0.01f, -0.01f, -0.00f, 0.02f, 0.01f, 0.01f, 0.00f, 0.00f,
                                 0.02f, 0.00f, -0.02f, 0.00f, 0.01f, 0.02f, 0.01f, -0.02f, -0.04f, -0.04f, -0.02f, -0.05f, -0.05f, -0.05f,
                                 -0.05f, -0.05f, -0.05f, -0.05f, -0.05f, -0.05f, -0.05f, -0.03f, -0.04f, -0.02f, 0.00f, 0.03f, 0.04f, 0.01f,
                                 0.02f, 0.03f, 0.04f, 0.05f, 0.05f, 0.04f, 0.03f, 0.05f, 0.03f, 0.03f, 0.05f, 0.05f, 0.04f, 0.05f,
                                 0.03f, 0.03f, 0.02f, 0.02f, 0.03f, 0.02f, 0.01f, 0.02f, 0.03f, 0.01f, 0.03f, 0.02f, 0.05f, 0.04f,
                                 0.04f, 0.02f, 0.02f, 0.00f, 0.01f, -0.00f, 0.02f, -0.01f, -0.01f, 0.00f, 0.00f, -0.00f, -0.02f, 0.01f,
                                 0.01f, 0.03f, 0.05f, 0.04f, 0.01f, 0.01f, 0.03f, 0.04f, 0.05f, 0.05f, 0.05f, 0.05f, 0.03f, 0.02f,
                                 0.00f, -0.00f, -0.01f, -0.02f, 0.00f, 0.00f, 0.00f, -0.02f, -0.01f, 0.01f, 0.01f, 0.01f, 0.02f, 0.04f,
                                 0.03f, 0.03f, 0.05f, 0.05f, 0.04f, 0.01f, 0.03f, 0.04f, 0.05f, 0.03f, 0.01f, 0.01f, 0.04f, 0.05f,
                                 0.05f, 0.05f, 0.05f, 0.05f, 0.04f, 0.02f, 0.02f, 0.02f, 0.00f, 0.01f, 0.00f, 0.01f, 0.01f, 0.02f,
                                 0.04f, 0.02f, -0.00f, -0.01f, 0.00f, 0.02f, 0.05f, 0.03f, 0.04f, 0.03f, 0.01f, 0.03f, 0.05f, 0.04f,
                                 0.01f, 0.02f, 0.02f, 0.02f, 0.05f, 0.02f, 0.02f, -0.01f, 0.00f, -0.02f, -0.04f, -0.05f, -0.05f, -0.05f,
                                 -0.05f, -0.05f, -0.04f, -0.05f, -0.04f, -0.04f, -0.03f, -0.03f, -0.05f, -0.05f, -0.03f, -0.04f, -0.05f, -0.05f,
                                 -0.05f, -0.03f, -0.01f, -0.02f, -0.04f, -0.02f, -0.04f, -0.02f, -0.04f, -0.02f, -0.02f, 0.00f, -0.00f, 0.02f,
                                 0.00f, 0.03f, 0.01f, 0.01f, 0.01f, 0.02f, 0.04f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.03f,
                                 0.03f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.03f, 0.03f, 0.04f, 0.05f, 0.03f, 0.03f, 0.04f,
                                 0.02f, 0.01f, 0.02f, -0.00f, -0.00f, 0.01f, -0.00f, 0.02f, 0.02f, 0.02f, 0.04f, 0.03f, 0.01f, 0.02f,
                                 0.02f, 0.04f, 0.04f, 0.05f, 0.05f, 0.04f, 0.02f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, -0.01f, 0.02f};

    /** Index counter to iterate over lightVary array. */
    int lightVaryIter;

public:

    /** Default constructor.
     *  Does not call Model constructor as is currently only used in Raft's model hierarchy, i.e.
     *  it doesn't need an independent location vector. */
    Lantern() : lightVaryIter{0} {}

    void draw() const;
    void update();

};


#endif //GRAPHICS_CW_LANTERN_H
