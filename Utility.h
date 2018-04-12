//
// Created by Edward Gargan on 12/04/2018.
//

#ifndef GRAPHICS_CW_UTILITY_H
#define GRAPHICS_CW_UTILITY_H

#include <glut/glut.h>

/** Calls gl methods to setup a surface that reflects light -- make sure to push + pop attribs either side!  */
inline void materialise(float amb[], float dif[], float spec[])  {

    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);

    glMaterialf(GL_FRONT, GL_SHININESS, 128.0f);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);


    // TODO: look at shininess as parameter, necessary??

}


inline float* normalrgb(int r, int g, int b) {

    return (float[]){(float)r / 255,
                     (float)g / 255,
                     (float)b / 255};

}




#endif //GRAPHICS_CW_UTILITY_H
