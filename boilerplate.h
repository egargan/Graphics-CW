//
// Created by Edward Gargan on 09/04/2018.
//

#ifndef COURSEWORK_BOILERPLATE_H
#define COURSEWORK_BOILERPLATE_H

#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include "boilerplate.h"

#include <glut/glut.h>
#include <cstdio>

int height, width;

void checkGLError() {

    int e = 0;
    GLenum error = glGetError();

    while (GL_NO_ERROR != error) {
        e++;
        printf("GL Error %i: %s\n", e, gluErrorString(error));      // Display error string
        error = glGetError();                                       // Get next glError
    }
}

void setup() {

    width = 800;                                    // initialise global window variables
    height = 800;                                   // define in your header: int width, height;

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);  // enable 3D rendering and double buffering
    glutInitWindowSize(width, height);              // set window size
    glutCreateWindow("Coursework");                 // create and show window (named MyScene)

}

void reshape(int _width, int _height) {

    height = _height;
    width = _width;

    // Get new aspect ratio
    GLdouble aspect = static_cast<GLdouble>(width) / static_cast<GLdouble>(height);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();  // reset matrix
    gluPerspective(45.0, aspect, 1, 1000);

    glMatrixMode(GL_MODELVIEW);  // Return matrix mode to modelling and viewing
}



#endif //COURSEWORK_BOILERPLATE_H
