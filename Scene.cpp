
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include "boilerplate.h"
#include "model/Water.h"
#include "model/Crate.h"


Water* wart;
Crate* box;


void makeLight(float pos[], GLenum lr) {

    //glLightfv(lr, GL_SPOT_DIRECTION, (float[]){0.f, -1.f, 1.f});
    //glLightf(lr, GL_SPOT_EXPONENT, 8.f);
    //glLightf(lr, GL_SPOT_CUTOFF, 45.f);

    glLightfv(lr, GL_DIFFUSE,  (float[]) {160/255.f, 160/255.f, 160/255.f});
    glLightfv(lr, GL_SPECULAR, (float[]) {240/255.f, 207/255.f, 135/255.f});

    glLightfv(lr, GL_POSITION, (GLfloat[]){pos[0], pos[1], pos[2]});

    glEnable(lr);

}


void draw() {

    glLoadIdentity();

    glClearColor(0.2f, 0.4f, 0.8f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);

    // Tell OpenGL to order drawing according to depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    gluLookAt(  4.f, 15.f, 60.f,  // Camera postition
                0.f,  0.f, 0.f,    // 'Look-at' position
                0.f,  1.f, 0.f );  // Orientation (unit vector) of 'top' of camera


    wart->update();
    wart->draw();

    box->update();
    box->draw();


    makeLight((float[]) {0.f, 10.f, -50.f}, GL_LIGHT0);

    glDisable(GL_LIGHTING);

    glPushMatrix();

    glTranslatef(0.f, 10.f, -50.f);
    glutSolidSphere(1.2, 10, 10);   // Draw ball at light source

    glPopMatrix();


    glutSwapBuffers(); // Swap double buffers

}


void init() {

    // Init models + textures

    // TODO: seg fault for small tile size, perhaps FloatingModel getting mesh indices outside bounds?
    wart = new Water(1.0f, 100);

    box = new Crate(wart, Vec3f{-5.f, 0.f, 12.f}, 3);

}


int main(int argc, char **argv) {

    glutInit(&argc, argv);          // Initialise GL environment
    setup();                        // Call additional initialisation commands

    glutDisplayFunc(draw);          // Register scene to render contents of draw() function
    glutIdleFunc(draw);

    checkGLError();                 // Check any OpenGL errors in initialisation

    init();

    glutReshapeFunc(reshape);
    glutMainLoop();                 // Begin rendering sequence


    return 0;

}

