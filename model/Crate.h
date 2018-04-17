//
// Created by Edward Gargan on 17/04/2018.
//

#ifndef GRAPHICS_CW_CRATE_H
#define GRAPHICS_CW_CRATE_H

#include "FloatingModel.h"

class Crate : public FloatingModel {

public:

    Crate(Water* water, Vec3f _location, float _width, float _length);

    void draw() const;


};


#endif //GRAPHICS_CW_CRATE_H
