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
void makeLight(float pos[], GLenum lr);

#endif //GRAPHICS_CW_UTILITY_H
