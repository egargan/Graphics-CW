
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include "boilerplate.h"
#include "Utility.h"

#include "model/Water.h"
#include "model/Crate.h"
#include "model/Raft.h"


// Scene models
Water* wart;
Crate* box;
Raft* raft;


// Offset values for 'gluLookAt' call, added to look at position to look around scene
float lookatOffsetX, lookatOffsetY;

// Field of vue of look around
const float fov = 8.f;


// Rotation value about X axis for rising/setting sun, controlled by keyboard input
float sunrot = 20.f;

// Speed of sun's movement in response to key input
const float sunvel = 0.2f;

// Distance of sun away from origin
const float sundist = 120.f;


void lightScene();


void draw() {

    glLoadIdentity();

    glClearColor(0.2f, 0.4f, 0.8f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);

    glShadeModel(GL_SMOOTH);

    // Tell OpenGL to order drawing according to depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    gluLookAt(  4.f, 15.f, 65.f,                                 // Camera postition
                0.f + lookatOffsetX,  0.f + lookatOffsetY, 0.f,  // 'Look-at' position
                0.f,  1.f, 0.f );                                // Orientation (unit vector) of 'top' of camera


    wart->update();
    wart->draw();

    box->update();
    box->draw();

    glRotatef(8, 0.f, 1.f, 0.f);

    raft->update();
    raft->draw();

    lightScene();


    glutSwapBuffers(); // Swap double buffers

}

/** Lights scene according to position of sun, to simulate real world lighting. */
void lightScene() {

    glPushMatrix();

    glRotatef(sunrot, 1.f, 0.f, 0.f);
    glTranslatef(0.f, 0.f, -sundist);

    makeLight((float[]) {0.f, 0.f, 0.f}, GL_LIGHT0);

    glDisable(GL_LIGHTING);

    glutSolidSphere(1.2, 10, 10);   // Draw ball at light source

    glPopMatrix();



}


void init() {

    lookatOffsetX = 0.f;
    lookatOffsetY = 0.f;

    // Init models

    // TODO: seg fault for small tile size, perhaps FloatingModel getting mesh indices outside bounds?2
    wart = new Water(1.8f, 110);

    box = new Crate(wart, Vec3f{-8.f, 0.f, 34.f}, 3);

    raft = new Raft(wart, Vec3f{6.f, 0.f, 32.f}, 17, 14, 15);

}


/** 'Passive' mouse callback given to opengl to handle all mouse movement within the window. */
void mouse(int x, int y) {

    if (x > 0 && x < windowwidth && y > 0 && y < windowheight) {

        lookatOffsetX = ((x / (float) windowwidth) - 0.5f) * (fov / 2.f);
        lookatOffsetY = - ((y / (float) windowheight) - 0.5f) * (fov / 2.f);
    }

}

void keyboard(int key, int, int) {

    switch (key) {

        case GLUT_KEY_LEFT :

            if (sunrot > -20.f) {
                sunrot -= sunvel;
            }
            break;

        case GLUT_KEY_RIGHT:

            if (sunrot < 110.0f) {
                sunrot += sunvel;
            }
            break;

        default:
            break;
    }

}


int main(int argc, char **argv) {

    glutInit(&argc, argv);          // Initialise GL environment
    setup();                        // Call additional initialisation commands

    init();

    glutDisplayFunc(draw);          // Register scene to render contents of draw() function
    glutIdleFunc(draw);

    glutPassiveMotionFunc(mouse);
    glutSpecialFunc(keyboard);

    checkGLError();                 // Check any OpenGL errors in initialisation

    glutReshapeFunc(reshape);
    glutMainLoop();                 // Begin rendering sequence


    return 0;

}

