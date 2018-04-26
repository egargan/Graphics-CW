
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include "boilerplate.h"
#include "Utility.h"

#include "model/allmodels.h"


// Scene models
Water* wart;
Crate* forebox;
Crate* backbox;
Raft* raft;


// Offset values for 'gluLookAt' call, added to look at position to look around scene
float lookatOffsetX, lookatOffsetY;

// Field of vue of look around
const float fov = 360.f;

float mov = 0.f; // TODO: remove

//Vec3f bgcol{15/255.f, 23/255.f, 47/255.f};
Vec3f bgcol{140/255.f, 198/255.f, 255/255.f};

int daynight = 0;


void lightScene();
void init();
void draw();
void mouse();
void keyboard();

// TODO: remove
float candlevar = 0.f;
int count = 0;


void draw() {

    glLoadIdentity();

    glClearColor(bgcol.x, bgcol.y, bgcol.z, 1.f);

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

    gluLookAt(  4.f, 15.f, 65.f + mov,                           // Camera postition
                -5.f + lookatOffsetX,  4.f + lookatOffsetY, 0.f, // 'Look-at' position
                0.f,  1.f, 0.f );                                // Orientation (unit vector) of 'top' of camera

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

        glTranslatef(-22.f, 25.f, -220.f);
        glDisable(GL_LIGHTING);

        glutSolidSphere(2, 8, 8);

        glEnable(GL_LIGHTING);

    glPopMatrix();

    if (daynight == 1) {

        // night lighting
        makeLight(GL_LIGHT0,
                  (float[]) {0.f, 0.f, 1.f},                         // direction
                  (float[]) {-22.f, 25.f, -220.f, 1.f},              // position
                  (float[3]) {0},                                    // ambient light
                  (float[]) {46/255.f, 56/255.f, 66/255.f, 1.f},     // diffuse
                  (float[]) {200/255.f, 181/255.f, 118/255.f, 1.f},  // specular
                  180.f,                                             // cutoff
                  1.4f);                                             // constant attenuation
    } else {

        // day lighting
        makeLight(GL_LIGHT0,
                  (float[]) {0.f, 0.f, 1.f},                               // direction
                  (float[]) {-22.f, 25.f, -220.f, 1.f},                    // position
                  (float[]) {130 / 255.f, 130 / 255.f, 130 / 255.f},       // ambient light
                  (float[]) {150 / 255.f, 189 / 255.f, 189 / 255.f, 1.f},  // diffuse
                  (float[]) {230 / 255.f, 215 / 255.f, 130 / 255.f, 1.f},  // specular
                  180.f,                                                   // cutoff
                  1.1f);                                                   // constant attenuation
    }

    glutSwapBuffers(); // Swap double buffers


    // TODO: remove
    //candlevar = candlevar - 0.025f + ((rand() % 11) / 200.f);
    //
    //if ((candlevar) > 0.05f) candlevar = 0.05f;
    //if ((candlevar) < -0.05f) candlevar = -0.05f;
    //
    //printf("%.2ff, ", candlevar);
    //
    //if (cunt++ > 12){printf("\n"); cunt = 0;}



}


void init() {

    lookatOffsetX = 0.f;
    lookatOffsetY = 0.f;

    // Init models

    // TODO: seg fault for small tile size, perhaps FloatingModel getting mesh indices outside bounds?
    wart = new Water(2.5f, 120);

    raft = new Raft(wart, Vec3f{6.f, 0.1f, 32.f}, 17, 14, 15);

    forebox =  new Crate(wart, Vec3f{-8.f, 0.f, 34.f}, 3);
    backbox =  new Crate(wart, Vec3f{-15.f, 0.f, 27.f}, 2.9);

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

            daynight = 0;
            bgcol = {140/255.f, 198/255.f, 255/255.f};

            mov++;

            break;

        case GLUT_KEY_RIGHT:

            daynight = 1;
            bgcol = {15/255.f, 23/255.f, 47/255.f};

            mov--;

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

