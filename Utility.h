//
// Created by Edward Gargan on 12/04/2018.
//

#ifndef GRAPHICS_CW_UTILITY_H
#define GRAPHICS_CW_UTILITY_H

#include <glut/glut.h>

#include <string>
#include <cstdio>
#include "Vec3.h"

/** Calls gl methods to setup a surface that reflects light -- make sure to push + pop attribs either side!  */
void materialise(float amb[], float dif[], float spec[], float shine);

/** Reads BMP file at given path and binds image data to returned texture ID.
 * @return Texture ID of loaded texture.*/
int loadBMP(const std::string& path);

/** Calls lighting methods for given light index (e.g. GL_LIGHT0) at given position. */
inline void makeLight(GLenum lr,
                      float dir[], float pos[], float amb[],
                      float dif[], float spec[],
                      float cutoff, float atten) {

    glLightfv(lr, GL_SPOT_DIRECTION, dir);
    glLightf(lr, GL_SPOT_CUTOFF, cutoff);
    glLightf(lr,  GL_SPOT_EXPONENT, 8.f);

    glLightf(lr,  GL_CONSTANT_ATTENUATION, atten);

    glLightfv(lr, GL_POSITION, (GLfloat[]) {pos[0], pos[1], pos[2], pos[3]}); // 1.0f == directed light

    glLightfv(lr, GL_AMBIENT,  amb);
    glLightfv(lr, GL_DIFFUSE,  dif);
    glLightfv(lr, GL_SPECULAR, spec);

    glEnable(lr);

}

/** Linear interpolate between a and b, by degree t. */
inline float lerp(float a, float b, float t) {
    return (b - a) * t + a;
}

#endif //GRAPHICS_CW_UTILITY_H
