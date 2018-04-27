
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include "boilerplate.h"
#include "Utility.h"

#include "model/allmodels.h"
#include "model/Sky.h"


// Scene models
Water* wart;
Crate* forebox;
Crate* backbox;
Raft* raft;
Sky* sky;


// Offset values for 'gluLookAt' call, added to look at position to look around scene
float lookatOffsetX, lookatOffsetY;

// Field of vue of look around
const float fov = 5.f;


void lightScene();
void init();
void draw();
void mouse();
void keyboard();



void draw() {

    glLoadIdentity();

    //glClearColor(bgcol.x, bgcol.y, bgcol.z, 1.f);
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Tell OpenGL to order drawing according to depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHTING);
    glShadeModel(GL_FLAT);

    gluLookAt(  4.f, 15.f, 65.f,                              // Camera postition
                2.f + lookatOffsetX, 4.f + lookatOffsetY, 0.f,      // 'Look-at' position
                0.f,  1.f, 0.f );                             // Orientation (unit vector) of 'top' of camera

    glPushMatrix();

        glTranslatef(0.f, 0.f, -40.f);
        wart->update();
        wart->draw();

    glPopMatrix();

    forebox->update();
    forebox->draw();

    backbox->update();
    backbox->draw();

    glPushMatrix();

        glRotatef(8, 0.f, 1.f, 0.f);

        raft->update();
        raft->draw();

    glPopMatrix();

    glPushMatrix();

    glPopMatrix();

    sky->update();
    sky->draw();

    glutSwapBuffers(); // Swap double buffers

}


void init() {

    lookatOffsetX = 0.f;
    lookatOffsetY = 0.f;

    // Init models
    sky = new Sky(400.f, 400.f, {0.f, 0.f, -150.f});

    wart = new Water(2.5f, 120);

    raft = new Raft(wart, Vec3f{10.f, 0.1f, 32.f}, 17, 14, 15);

    forebox =  new Crate(wart, Vec3f{-4.f, 0.f, 34.f}, 3.2);
    backbox =  new Crate(wart, Vec3f{-11.f, 0.f, 27.f}, 2.9);

}


/** 'Passive' mouse callback given to opengl to handle all mouse movement within the window. */
void mouse(int x, int y) {

    if (x > 0 && x < windowwidth && y > 0 && y < windowheight) {

        lookatOffsetX =  ((x / (float) windowwidth)  - 0.5f) * (fov / 2.f);
        lookatOffsetY = -((y / (float) windowheight) - 0.5f) * (fov / 2.f);
    }

}

void keyboard(int key, int, int) {

    switch (key) {

        case GLUT_KEY_LEFT :

            sky->transition();
            break;

        case GLUT_KEY_RIGHT:

            sky->transition();
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

    printf("Version: %s\n", glGetString(GL_VERSION));

    glutReshapeFunc(reshape);
    glutMainLoop();                 // Begin rendering sequence


    return 0;

}

