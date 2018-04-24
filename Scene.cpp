
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include "boilerplate.h"
#include "Utility.h"

#include "model/allmodels.h"


// Scene models
Water* wart;
Crate* box;
Raft* raft;


// Offset values for 'gluLookAt' call, added to look at position to look around scene
float lookatOffsetX, lookatOffsetY;

// Field of vue of look around
const float fov = 8.f;


float count = 0.1f;


Vec3f bgcol{17/255.f, 17/255.f, 33/255.f};


void lightScene();
void init();
void draw();
void mouse();
void keyboard();

void draw() {

    glLoadIdentity();

    glClearColor(bgcol.x, bgcol.y, bgcol.z, 1.f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);

    // Tell OpenGL to order drawing according to depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    gluLookAt(  4.f, 15.f, 65.f,                                 // Camera postition
                -5.f + lookatOffsetX,  4.f + lookatOffsetY, 0.f,  // 'Look-at' position
                0.f,  1.f, 0.f );                                // Orientation (unit vector) of 'top' of camera


    glPushMatrix();

        glTranslatef(0.f, 0.f, -40.f);
        wart->update();
        wart->draw();

    glPopMatrix();

    box->update();
    box->draw();

    glPushMatrix();

        glRotatef(8, 0.f, 1.f, 0.f);

        raft->update();
        raft->draw();

    glPopMatrix();

    makeLight(GL_LIGHT0,
              (float[]) {sin(count), 0.f, cos(count)},          // direction
              (float[]) {-20.f, 10.f, -70.f, 1.f},               // position
              (float[3]) {0.f},                                 // ambient light
              (float[]) {60/255.f, 54/255.f, 35/255.f, 1.f}, // diffuse
              (float[]) {190/255.f, 180/255.f, 130/255.f, 1.f}, // specular
              89.f,                                             // cutoff
              1.0f);                                            // constant attenuation

    makeLight(GL_LIGHT1,
              (float[]) {sin(count), 0.f, cos(count)},
              (float[]) {-20.f, 10.f, -70.f, 1.f},              // position
              (float[]) {3/255.f, 4/255.f, 5/255.f, 1.f},       // ambient light
              (float[]) {170/255.f, 160/255.f, 110/255.f, 1.f}, // diffuse
              (float[]) {80/255.f, 70/255.f, 30/255.f, 1.f},    // specular
              30.f,                                             // cutoff
              4.f);                                             // constant attenuation

    glutSwapBuffers(); // Swap double buffers

}


void init() {

    lookatOffsetX = 0.f;
    lookatOffsetY = 0.f;

    // Init models

    // TODO: seg fault for small tile size, perhaps FloatingModel getting mesh indices outside bounds?
    wart = new Water(2.5f, 110);
    box =  new Crate(wart, Vec3f{-8.f, 0.f, 34.f}, 3);
    raft = new Raft(wart, Vec3f{6.f, 0.f, 32.f}, 17, 14, 15);

}


/** 'Passive' mouse callback given to opengl to handle all mouse movement within the window. */
void mouse(int x, int y) {

    if (x > 0 && x < windowwidth && y > 0 && y < windowheight) {

        lookatOffsetX =  ((x / (float) windowwidth) - 0.5f) * (fov / 2.f);
        lookatOffsetY = -((y / (float) windowheight) - 0.5f) * (fov / 2.f);
    }

}

void keyboard(int key, int, int) {

    switch (key) {

        case GLUT_KEY_LEFT :

            count -= 0.02f;
            break;

        case GLUT_KEY_RIGHT:

            count += 0.02f;
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

