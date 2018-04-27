//
// Created by Edward Gargan on 26/04/2018.
//

#ifndef GRAPHICS_CW_SKY_H
#define GRAPHICS_CW_SKY_H

#include <GLUT/glut.h>
#include "../Vec3.h"

/**
 * Displays rotate-able 'Sky' shape.
 *
 * Draws two stacked 2D rectangles, one coloured with a daytime sky colour, the other a dark nighttime colour.
 * This shape rotates about the z-axis to present one colour or the other.
 *
 * Also creates spheres and light objects representing the sun and moon.
 */
class Sky : Model {

    // Sky dimensions
    float width;
    float height;

    // Sky background colours
    const Vec3f daycolr{140/255.f, 198/255.f, 255/255.f};
    const Vec3f nightcolr{15/255.f, 23/255.f, 47/255.f};

    // Sunlight colours
    const Vec3f dayamb{130 / 255.f, 130 / 255.f, 130 / 255.f};
    const Vec3f daydif{150 / 255.f, 189 / 255.f, 189 / 255.f};
    const Vec3f dayspec{230 / 255.f, 215 / 255.f, 130 / 255.f};

    // Moonlight colours
    const Vec3f nightamb{2/255.f, 2/255.f, 2/255.f};
    const Vec3f nightdif{46/255.f, 56/255.f, 66/255.f};
    const Vec3f nightspec{200/255.f, 181/255.f, 118/255.f};

    /** Pairs of x/y coordinates of dots representing stars on the night sky. */
    const std::vector<float> starPositions = {0.32, 0.33,
                                              0.71,  0.23,
                                              0.49, 0.34,
                                              0.42, 0.21,
                                              0.46, 0.19,
                                              0.39, 0.14,
                                              0.47, 0.08,
                                              0.48, 0.44,
                                              0.59, 0.32,
                                              0.46, 0.44,
                                              0.61, 0.21};

    /** Location of sun/moon relative to sky's location. */
    const Vec3f lightpos{5.f, 30.f, 0.f}; // Hard coded due to time constraint :(

    /** Boolean holding true if it's currently 'daytime', i.e. day-coloured rectangle is on top, or nighttime. */
    bool daytime;

    /** Current rotation value, used to display either day or night half of sky shape. */
    float rot;

    /** Represents sky's current transition state, i.e. 1 is complete transition. */
    float transtate;

    /** Rate of change of rotation when transitioning between day and night. */
    const float rotspeed = 4.f;


public:

    /** Constructs Sky model with given dimensions and location. */
    Sky(const float _width, const float _height, Vec3f location) :
            Model(location),
            width{_width}, height{_height},
            daytime{true},
            rot{0.f}, transtate{1.f}
    {}

    /** Draws day and night sky rectangles according to current rotation, and creates sun or moon
     *  light sources according to 'daytime' boolean.
     *  Sun, moon and star models (i.e. glutSpheres) are drawn on top of day/night sky layers. */
    void draw() const {

        // Holders for halves of sky dimension values
        const float halfwidth = width / 2;
        const float halfheight = height / 2;

        glDisable(GL_LIGHTING);

        glPushMatrix();

            // Translate to sky's given location
            Model::doTransform();

            glRotatef(daytime ? rot : -rot, 0.f, 0.f, 1.f);

            // Slightly translate so other sky colour doesn't peep over waves
            glTranslatef(0.f, (daytime ? -2.f : 2.f) * transtate, 0.f);

            glBegin(GL_QUADS);

                glNormal3f(0.f, 0.f, 1.f);

                glColor3f(daycolr.x, daycolr.y, daycolr.z);

                glVertex3f(-halfwidth, halfheight, 0.f);
                glVertex3f(-halfwidth, 0.f, 0.f);
                glVertex3f(halfwidth, 0.f, 0.f);
                glVertex3f(halfwidth, halfheight, 0.f);

                glColor3f(nightcolr.x, nightcolr.y, nightcolr.z);

                glVertex3f(-halfwidth, 0.f, 0.f);
                glVertex3f(-halfwidth, -halfheight, 0.f);
                glVertex3f(halfwidth, -halfheight, 0.f);
                glVertex3f(halfwidth, 0.f, 0.f);

            glEnd();


            glPushMatrix(); // Draw sun and moon

                glTranslatef(lightpos.x, lightpos.y, lightpos.z);

                glColor3f(1.f, 1.f, 1.f);
                glutSolidSphere(4, 10, 10);

                glTranslatef(-lightpos.x * 2, -lightpos.y * 2, -lightpos.z * 2);

                glColor3f(1.f, 1.f, 0.9f);
                glutSolidSphere(2, 8, 8);

            glPopMatrix();


            glColor3f(1.f, 1.f, 1.f);

            // Draw small spheres representing stars according to (x, y) coords in 'starPositions'
            for (auto iter = starPositions.cbegin(); iter < starPositions.cend(); std::advance(iter, 2)) {

                glPushMatrix();
                    glTranslatef((*iter - 0.5f) * width , *(iter+1) * -halfheight, 0.f);
                    glutSolidSphere(0.2f, 4, 4);
                glPopMatrix();
            }

            // Only draw one of sun or moon light source -- drawing both would result in light 'leaking' through
            // from below the water's horizon, especially daylight's ambient light.
            if (daytime) {

                makeLight(GL_LIGHT0,
                          (float[]) {0.f, 0.f, 1.f},
                          (float[]) {location.x + lightpos.x, location.y + lightpos.y, location.z + lightpos.z, 1.f},
                          (float[]) {dayamb.x * transtate, dayamb.y * transtate, dayamb.z * transtate, 1.f},
                          (float[]) {daydif.x * transtate, daydif.y * transtate, daydif.z * transtate, 1.f}, 
                          (float[]) {dayspec.x * transtate, dayspec.y * transtate, dayspec.z * transtate, 1.f},  
                          180.f,
                          1.1f);

            } else {

                makeLight(GL_LIGHT0,
                          (float[]) {0.f, 0.f, 1.f},
                          (float[]) {location.x - lightpos.x, location.y - lightpos.y, location.z - lightpos.z, 1.f},
                          (float[]) {nightamb.x * transtate, nightamb.y * transtate, nightamb.z * transtate, 1.f},
                          (float[]) {nightdif.x * transtate, nightdif.y * transtate, nightdif.z * transtate, 1.f},
                          (float[]) {nightspec.x * transtate, nightspec.y * transtate, nightspec.z * transtate, 1.f},
                          180.f,
                          1.8f);
            }

        glPopMatrix();

        glEnable(GL_LIGHTING);

    }

    /** Performs any current transition between night and day. */
    void update() {

        if (transtate == 1.f) return;

        if (rot > 0 && daytime) {

            rot -= rotspeed;
            transtate = (1 - rot / 180.f);

        } else if (rot < 180 && !daytime) {

            rot += rotspeed;
            transtate = rot / 180.f;
        }

    }

    /** Rotates sky figure 180 degrees about z-axis to 'switch' between night and day. */
    inline void transition() {

        // If not currently transitioning, then ignore.
        if (transtate == 1.f) {

            transtate = 0.f;
            daytime = !daytime;
        }
    }

};


#endif //GRAPHICS_CW_SKY_H
