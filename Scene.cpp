
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include "boilerplate.h"


void draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(0.2f, 0.2f, 0.2f, 1.f);

    // make beautiful models

    glutSwapBuffers(); // Swap double buffers

}


int main(int argc, char **argv) {

    glutInit(&argc, argv);          // Initialise GL environment
    setup();                        // Call additional initialisation commands

    glutDisplayFunc(draw);          // Register scene to render contents of draw() function

    checkGLError();                 // Check any OpenGL errors in initialisation

    glutReshapeFunc(reshape);
    glutMainLoop();                 // Begin rendering sequence

    return 0;

}

