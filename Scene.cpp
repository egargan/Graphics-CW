
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include "boilerplate.h"
#include "Water.h"

Water* wart;

void makeLight(float pos[], GLenum lr) {

    //glLightfv(lr, GL_SPOT_DIRECTION, (float[]){0.f, -1.f, 1.f});
    //glLightf(lr, GL_SPOT_EXPONENT, 8.f);
    //glLightf(lr, GL_SPOT_CUTOFF, 45.f);

    glLightfv(lr, GL_DIFFUSE,  (float[]){240/255.f, 207/255.f, 135/255.f});
    glLightfv(lr, GL_SPECULAR, (float[]){240/255.f, 207/255.f, 135/255.f});

    glLightfv(lr, GL_POSITION, (GLfloat[]){pos[0], pos[1], pos[2]});

    glEnable(lr);

}

float rot = 0.f;

void draw() {

    glLoadIdentity();

    glClearColor(0.1f, 0.2f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);

    // Tell OpenGL to order drawing according to depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    gluLookAt(  0.f, 10.f, 35.f,  // Camera postition
                0.f, 0.f, 0.f,    // 'Look-at' position
                0.f, 1.f, 0.f );  // Orientation (unit vector) of 'top' of camera

    wart->draw();
    wart->update();

    //glRotatef(rot+=0.5, 1.f, 0.f, 0.f);

    makeLight((float[]) {0.f, 10.f, -20.f}, GL_LIGHT0);

    glDisable(GL_LIGHTING);

    glTranslatef(0.f, 10.f, -20.f);
    glutSolidSphere(1.2, 10, 10);   // Draw ball at light source

    glutSwapBuffers(); // Swap double buffers

}


int main(int argc, char **argv) {

    wart = new Water(0.5f,100);

    glutInit(&argc, argv);          // Initialise GL environment
    setup();                        // Call additional initialisation commands

    glutDisplayFunc(draw);          // Register scene to render contents of draw() function
    glutIdleFunc(draw);

    checkGLError();                 // Check any OpenGL errors in initialisation

    glutReshapeFunc(reshape);
    glutMainLoop();                 // Begin rendering sequence

    return 0;

}

