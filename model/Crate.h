//
// Created by Edward Gargan on 17/04/2018.
//

#ifndef GRAPHICS_CW_CRATE_H
#define GRAPHICS_CW_CRATE_H

#include "FloatingModel.h"
#include "../Utility.h"

class Crate : public FloatingModel {

    /** ID of crate texture. */
    unsigned int texId;

public:

    /** Crate constructor accepting dimensions. */
    Crate(Water* water, Vec3f _location, float _width);

    void draw() const;

};


#endif //GRAPHICS_CW_CRATE_H
