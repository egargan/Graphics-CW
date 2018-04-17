//
// Created by Edward Gargan on 17/04/2018.
//

#include "Crate.h"

Crate::Crate(Water *water, Vec3f _location, float _width, float _length) :
        FloatingModel(water, _location, _width, _length) {

}

void Crate::draw() const {

    glPushMatrix();

    doTransform();

    glutSolidCube(width);

    glPopMatrix();

}

