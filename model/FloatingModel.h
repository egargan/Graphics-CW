//
// Created by Edward Gargan on 13/04/2018.
//

#ifndef GRAPHICS_CW_FLOATINGMODEL_H
#define GRAPHICS_CW_FLOATINGMODEL_H

#include "Water.h"

class FloatingModel : public Model {

protected:

    /** Rotation angles about X and Z axis used for model's 'bobbing' animation. */
    float rotX, rotZ;

    /** Pointer to water on which model floats. */
    Water* surface;

    /** Dimensions of model segment that contacts water. */
    float width, length;

    /** 3 sets of water mesh coordinates describing triangle that approximates contact area. */
    int contactPoints[6];

    /** 'Bob' displacement of model on water layer. */
    float bob;

    FloatingModel(Water* water, Vec3f _location, float _width, float _length) :
            Model(_location),
            surface{water}, width{_width}, length{_length}, rotX{0.f}, rotZ{0.f}, contactPoints{0}, bob{0.f}
    {
        updateContactPoints(); // Get initial points of contact with water

        update(); // Get initial orientation.
    };

public:

    /** Update orientation vector based on y-displacement of 3 points approximating the contact area with water. */
    inline void update() override {

        Vec3f orientation = surface->getSurfaceNormal(
                contactPoints[0], contactPoints[1],
                contactPoints[2], contactPoints[3],
                contactPoints[4], contactPoints[5]);

        // TODO: this isn't right! need to get X and Z rotations that map (0,1,0) to orientation vector.
        rotX = 90 - atan2(orientation.y, orientation.x) * (180/M_PI);
        rotZ = 90 - atan2(orientation.y, orientation.z) * (180/M_PI);

        // Get average of contact points' Y components
        bob =  (surface->getMeshY(contactPoints[0],contactPoints[1]),
                surface->getMeshY(contactPoints[2],contactPoints[3]),
                surface->getMeshY(contactPoints[4],contactPoints[5])) / 3;

    }

    /** Performs transformations to position model ready for rendering. */
    inline void doTransform() const {

        // '* 2' exaggerates bob
        glTranslatef(location.x, location.y + (bob * 2), location.z);

        glRotatef(rotX, 1.f, 0.f, 0.f);
        glRotatef(rotZ, 0.f, 0.f, 1.f);

    }


    /** Populates contact points array according to model position.
     *
     *  A 3 coordinates, i.e. water mesh indices, describing a triangle that approximates
     *  the model's base on are derived from the location, width, and height of the model
     *  (assuming the model is level with the water's surface).
     *
     *  Should be called when model location changes across water's plane, e.g. in the x / z directions.*/
    inline void updateContactPoints() {

        const float tilelen = surface->getTileLength();
        const float halflen = tilelen / 2;

        const float halfnum = surface->getMeshLength() / 2;

        Vec3f gridPos = location / tilelen;

        gridPos.x += halfnum;
        gridPos.z += halfnum;

        contactPoints[0] = (int) std::round(gridPos.x);
        contactPoints[1] = (int) std::round(gridPos.z - length / halflen);
        contactPoints[2] = (int) std::round(gridPos.x - width  / halflen);
        contactPoints[3] = (int) std::round(gridPos.z + length / halflen);
        contactPoints[4] = (int) std::round(gridPos.x + width  / halflen);
        contactPoints[5] = (int) std::round(gridPos.z + length / halflen);

    }

};



#endif //GRAPHICS_CW_FLOATINGMODEL_H
