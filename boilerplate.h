//
// Created by Edward Gargan on 09/04/2018.
//

#ifndef COURSEWORK_BOILERPLATE_H
#define COURSEWORK_BOILERPLATE_H

#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#include <cstdio>

// Boilerplate code for initializing OpenGL environment.


int windowheight, windowwidth;

/** Check for and print any errors in OpenGL's initialisation. */
void checkGLError() {

    int e = 0;
    GLenum error = glGetError();

    while (GL_NO_ERROR != error) {
        e++;
        printf("GL Error %i: %s\n", e, gluErrorString(error));      // Display error string
        error = glGetError();                                       // Get next glError
    }
}

void windowsetup() {

    // initialise global window variables
    windowwidth = 900;
    windowheight = 800;

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);    // enable 3D rendering and double buffering
    glutInitWindowSize(windowwidth, windowheight);    // set window size

    glutCreateWindow("Coursework");

}

/** Window resize callback.
 * Refreshes perspective projection according to new window dimensions. */
void reshape(int _width, int _height) {

    windowheight = _height;
    windowwidth = _width;

    // Get new aspect ratio
    GLdouble aspect = static_cast<GLdouble>(windowwidth) / static_cast<GLdouble>(windowheight);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();  // reset matrix
    gluPerspective(45.0, aspect, 1, 1000);

    glMatrixMode(GL_MODELVIEW);  // Return matrix mode to modelling and viewing
}



#endif //COURSEWORK_BOILERPLATE_H
