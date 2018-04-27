//
// Created by Edward Gargan on 21/04/2018.
//

#include "Raft.h"
#include "../Utility.h"

/** Constructs raft model and loads relevant textures.
 *
 * @param water      Pointer to Water object on which the raft is supposed to float.
 * @param _location  Raft's location coordinates, vector of (x, y, z).
 * @param _width     Width of raft in x-direction.
 * @param _length    Length of raft in z-direction.
 * @param height     Height of raft's mast in y-direction.
 */
Raft::Raft(Water *water, const Vec3f _location, const float _width, const float _length, const float height) :

        FloatingModel(water, _location, _width, _length),
        mastHeight{height},
        varylistsize{3} // Initialize to be equal to the number of available log textures
{

    // Logs will be approximately (double) this size, adjusted slightly to exactly fit width
    const float idealrad = 0.6f;
    const float idealgap = idealrad / 5.f;

    baseLogRadius = (idealrad + (fmod(width, idealrad) / (floor(width / idealrad))));
    numBaseLogs =  (int) std::round(width / ((baseLogRadius * 2) + idealgap));

    varylist = new float[numBaseLogs] {0};

    // For each base log, create a random number between 0 and 1
    for (int i = 0, srand(35); i < numBaseLogs; i++) {
        varylist[i] = (rand() % 20) / 20.f;
    }

    logTexIds = new int[varylistsize] {0};

    // Load log textures
    logTexIds[0] = loadBMP("../textures/log/log_oak.bmp");
    logTexIds[1] = loadBMP("../textures/log/log_oak2.bmp");
    logTexIds[2] = loadBMP("../textures/log/log_spruce.bmp");

    logEndTexId = loadBMP("../textures/log/log_end.bmp");

    lantern = new Lantern();

}

/** De-point and delete heap objects. */
Raft::~Raft() {

    lantern = nullptr;
    varylist = nullptr;

    delete lantern;
    delete varylist;

}

/** Updates self's position according to FloatingModel's update(), and lantern model. */
void Raft::update() {

    FloatingModel::update();
    lantern->update();
}


// TODO: fill out comment

/**
 *
 * @param radius  Radius of log in x-direction, i.e. half its cross-section's width.
 * @param length  Length of log in z-direction.
 * @param varyInd Index in raft's 'vary list', used to add randomness to log colour.
 */
void Raft::drawLog(const float radius, const float length, const int varyInd) const {

    int res = 8; // Number of quads comprising cylinder

    float period = (float) (M_PI * 2) / res; // Increment of draw rotation, i.e. full rotation / res
    float x, y;

    Vec3f norm{}; // Surface normal holder

    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, (GLuint) logTexIds[(int)(varylist[varyInd] * 3) % 3]);

    // 'GL_MODULATE' multiples light color by texture color
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Give material lighting properties that combine with texture
    materialise((float[]){0.5f + varylist[varyInd] * 0.1f,  0.5f + varylist[varyInd] * 0.1f,  0.5f + varylist[varyInd] * 0.1f, 1.f},
                (float[]){0.5f + varylist[varyInd] * 0.1f, 0.5f + varylist[varyInd] * 0.1f,  0.5f + varylist[varyInd] * 0.1f, 1.f},
                (float[]){0.1f, 0.1f, 0.1f, 1.f},
                1.f);

    glBegin(GL_QUAD_STRIP); // Cylinder length

        for (int t = 0; t <= res; t++) {

            x = radius * sin(t * period);
            y = radius * cos(t * period);

            norm = {sin((t+0.5f) * period), cos((t+0.5f) * period), 0.f};
            norm /= norm.magnitude();

            glNormal3f(norm.x, norm.y, norm.z);

            glTexCoord2d(t/(float)res, 0);
            glVertex3f(x, y, 0.f);

            glTexCoord2d(t/(float)res, 1);
            glVertex3f(x, y, length);
        }

    glEnd();

    // Bind to the NULL texture buffer
    glBindTexture(GL_TEXTURE_2D, (GLuint) logEndTexId);

    glBegin(GL_TRIANGLE_FAN); // 'Far' lid for cylinder

        glNormal3f(0.f, 0.f, -1.f);

        glTexCoord2d(0.5f, 0.5f);
        glVertex3f(0.f, 0.f, 0.f); // Centre of triangle fan

        for (int t = 0; t <= res; t++) {
            glTexCoord2d(sin(t * period) * 0.5f + 0.5f, cos(t * period) * 0.5f + 0.5f);
            glVertex3f(radius * sin(t * period), radius * cos(t * period), 0.f);
        }

    glEnd();

    glBegin(GL_TRIANGLE_FAN); // Closer lid

        glNormal3f(0.f, 0.f, 1.f);

        glTexCoord2d(0.5f, 0.5f);
        glVertex3f(0.f, 0.f, length);

        for (int t = res; t >= 0; t--) {
            glTexCoord2d(sin(t * period) * 0.5f + 0.5f, cos(t * period) * 0.5f + 0.5f);
            glVertex3f(radius * sin(t * period), radius * cos(t * period), length);
        }

    glEnd();

    // Bind to NULL texture buffer
    glBindTexture(GL_TEXTURE_2D, (GLuint) NULL);
    glDisable(GL_TEXTURE_2D);

    glPopAttrib();

}

/** Draws raft's component models.
 *
 *  Draws a series of logs (cylinders) across the width of the raft, an upwards log representing a mast,
 *  and a lantern object, 'held up' by a couple of support logs attached to the mast.
 *
 * @see model/Lantern.cpp
 */
void Raft::draw() const {

    const float halfwidth = width / 2;
    const float halflength = length / 2;

    // Height of *position* of lantern above base
    const float lanternheight = mastHeight * 0.6f;

    // Horizontal distance of lantern away from mast / the length of the arm that supports it
    const float lanternjut = width * 0.25f;

    // Push new attribute state for lighting properties
    glPushAttrib(GL_SPECULAR | GL_DIFFUSE | GL_AMBIENT | GL_SHININESS);

    glPushMatrix();

        FloatingModel::doTransform();

        glPushMatrix(); // Push central position of raft, and draw logs making up floor of raft

            // Translate to far NW corner of raft
            glTranslatef(-halfwidth, 0.f, -halflength);

            for (int i = 0; i < numBaseLogs; i++, glTranslatef(baseLogRadius * 2.f + baseLogGap, 0.f, 0.f)) {

                glPushMatrix(); // Draw individual logs with slight variation in z direction

                    glTranslatef(0.f, 0.f, varylist[i]);
                    glRotatef(varylist[i] * 16, 0.f, 0.f, 1.f);

                    drawLog(baseLogRadius + varylist[i] * 0.2f, length + varylist[i] * 0.4f, i);

                glPopMatrix();
            }

        glPopMatrix();

        // Twist mast + attachments so is not exactly perpendicular to base logs
        glRotatef(21, 0.f, 1.f, 0.f);

        glPushMatrix(); // Draw central mast

            glRotatef(270, 1.f, 0.f, 0.f);

            drawLog(baseLogRadius * 0.9f, mastHeight, 2);

        glPopMatrix();


        glPushMatrix(); // Draw lantern arm, i.e. the log that 'holds' the lantern up

            // Translate to just above lantern, and right a bit so it appears to go through mast
            glTranslatef(width * 0.06f, lanternheight * 1.1f, 0.f);
            glRotatef(-90, 0.f, 1.f, 0.f);

            drawLog(baseLogRadius * 0.48f, lanternjut * 1.4f, 3);

        glPopMatrix();


        glPushMatrix();  // Draw lantern arm support

            // Translate to just below arm's position
            glTranslatef(0.f, lanternheight * 0.92f, 0.f);
            glRotatef(-90, 0.f, 1.f, 0.f);
            glRotatef(-47, 1.f, 0.f, 0.f);

            drawLog(baseLogRadius * 0.4f, mastHeight * 0.2f, 4);

        glPopMatrix();


        glPushMatrix(); // Draw lantern

            glTranslatef(-lanternjut, lanternheight, 0.f);

            // Undo floating model's 'bob' rotation so lamp is naturally stationary relative to raft
            glRotatef(-rotX, 1.f, 0.f, 0.f);
            glRotatef(-rotZ, 0.f, 0.f, 1.f);

            lantern->draw();

        glPopMatrix();


    glPopMatrix();

    glPopAttrib();

}
























