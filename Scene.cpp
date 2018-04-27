
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include "boilerplate.h"
#include "Utility.h"

#include "model/allmodels.h"


// Scene models
Water* water;
Sky* sky;
Crate* forebox;
Crate* backbox;
Raft* raft;


// Offset values for 'gluLookAt' call, added to look at position to look around scene
float lookatOffsetX, lookatOffsetY;

// Field of vue of look around
const float fovx = 5.f;
const float fovy = 10.f;


void init();
void draw();

void mouse(int, int);
void keyboard(unsigned char, int, int);



int main(int argc, char **argv) {

    glutInit(&argc, argv);          // Initialise GL environment
    windowsetup();                  // Call additional initialisation commands

    init();

    glutDisplayFunc(draw);          // Register scene to render contents of draw() function
    glutIdleFunc(draw);

    glutPassiveMotionFunc(mouse);
    glutKeyboardFunc(keyboard);     // Press space to switch between day and night!

    checkGLError();                 // Check any OpenGL errors in initialisation

    glutReshapeFunc(reshape);
    glutMainLoop();                 // Begin rendering sequence


    return 0;

}


/** Programs main draw loop.
 *  Enables appropriate OpenGL modes, adjusts camera, and updates and draws each model in scene. */
void draw() {

    glLoadIdentity();

    glClearColor(0.2f, 0.2f, 0.2f, 1.f); // Background colour -- should be hidden by sky model!

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Tell OpenGL to order drawing according to depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHTING);
    glShadeModel(GL_FLAT);

    gluLookAt(  4.f, 15.f, 65.f,                                 // Camera postition
                2.f + lookatOffsetX, 4.f + lookatOffsetY, 0.f,   // 'Look-at' position, offset by lookaround controls
                0.f,  1.f, 0.f );                                // Orientation (unit vector) of 'top' of camera

    water->update();
    forebox->update();
    backbox->update();
    raft->update();
    sky->update();

    glPushMatrix();

        // TODO: use water location instead.
        glTranslatef(0.f, 0.f, -40.f);
        water->draw();

    glPopMatrix();


    forebox->draw();
    backbox->draw();


    glPushMatrix();

        // Rotate box slightly, why not
        glRotatef(8, 0.f, 1.f, 0.f);

        raft->draw();

    glPopMatrix();

    sky->draw();


    glutSwapBuffers(); // Swap double buffers

}

/** Initialises scene's models and 'lookaround' values. */
void init() {

    lookatOffsetX = 0.f;
    lookatOffsetY = 0.f;

    water = new Water(2.5f, 120);
    sky = new Sky(400.f, 400.f, {0.f, 0.f, -150.f});

    forebox =  new Crate(water, Vec3f{-4.f, 0.f, 34.f}, 3.2);
    backbox =  new Crate(water, Vec3f{-11.f, 0.f, 27.f}, 2.9);

    raft = new Raft(water, Vec3f{10.f, 0.1f, 32.f}, 17, 14, 15);

}


/** 'Passive' mouse callback given to opengl to handle all mouse movement within the window. */
void mouse(int x, int y) {

    if (x > 0 && x < windowwidth && y > 0 && y < windowheight) {

        lookatOffsetX =  ((x / (float) windowwidth)  - 0.5f) * (fovx / 2.f);
        lookatOffsetY = -((y / (float) windowheight) - 0.5f) * (fovy / 2.f);
    }

}

/** Keyboard callback. If space is pressed, i.e. if 'key' == ' ', then transition between day and night in scene. */
void keyboard(unsigned char key, int, int) {

    switch (key) {

        case ' ' :
            sky->transition();

        default:
            break;
    }

}



