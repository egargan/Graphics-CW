
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include "boilerplate.h"
#include "Water.h"

Water* wart;

void draw() {

    glLoadIdentity();

    glClearColor(0.2f, 0.2f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    //glEnable(GL_LIGHTING);

    gluLookAt(  0.f, 1.f, 2.f,
                0.f, 0.f, 0.f,
                0.f, 1.f, 0.f );  // Camera oriented upwards

    wart->draw();
    wart->update();

    glutSwapBuffers(); // Swap double buffers

}

void init() {


}


int main(int argc, char **argv) {

    wart = new Water(30);

    glutInit(&argc, argv);          // Initialise GL environment
    setup();                        // Call additional initialisation commands

    glutDisplayFunc(draw);          // Register scene to render contents of draw() function
    glutIdleFunc(draw);

    checkGLError();                 // Check any OpenGL errors in initialisation

    glutReshapeFunc(reshape);
    glutMainLoop();                 // Begin rendering sequence

    return 0;

}

