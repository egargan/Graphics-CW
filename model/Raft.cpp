//
// Created by Edward Gargan on 21/04/2018.
//

#include "Raft.h"
#include "../Utility.h"

Raft::Raft(Water *water, const Vec3f _location, const float _width, const float _length, const float height) :
        FloatingModel(water, _location, _width, _length), mastHeight{height} {

    // logs will be approximately (double) this size, adjusted slightly to exactly fit width
    const float idealrad = 0.6f;
    const float idealgap = idealrad / 5.f;

    baseLogRadius = (idealrad + (fmod(width, idealrad) / (floor(width / idealrad))));

    numBaseLogs =  (int) std::round(width / ((baseLogRadius * 2) + idealgap));

    // TODO: account for log gap in calc!
    // baseLogRadius --

    // printf("width: %f, op: %f", width, (numBaseLogs * baseLogRadius * 2) + (loggap * (numBaseLogs - 1)));

    // Populate log colours array, one colour per log

    baseLogColours = new int[numBaseLogs] {0};

    srand(30);

    for (int i = 0; i < numBaseLogs; i++) {
        baseLogColours[i] = rand() % (int) browns.size();
    }

    lantern = new Lantern();

}

void Raft::drawLog(const float radius, const float length, const Vec3f colr) const {

    int res = 8; // Number of quads comprising cylinder

    float period = (float) (M_PI * 2) / res; // Increment of draw rotation, i.e. full rotation / res
    float x, y;

    Vec3f norm{}; // Surface normal holder

    materialise((float[]) {colr.x, colr.y, colr.z, 1.f},     // Ambient colour
                (float[]) {colr.x, colr.y, colr.z, 1.f},     // Diffuse
                (float[]) {colr.x, colr.y, colr.z, 1.f},     // Specular
                1.f);                                        // Shininess


    glBegin(GL_QUAD_STRIP); // Cylinder length

        for (int t = 0; t <= res; t++) {

            x = radius * sin(t * period);
            y = radius * cos(t * period);

            norm = {sin((t+0.5f) * period), cos((t+0.5f) * period), 0.f};
            norm /= norm.magnitude();

            glNormal3f(norm.x, norm.y, norm.z);

            glVertex3f(x, y, 0.f);
            glVertex3f(x, y, length);
        }

    glEnd();


    glBegin(GL_TRIANGLE_FAN); // 'Far' lid for cylinder

        // Make end of logs slightly lighter than log 'bark'
        materialise((float[]) {colr.x + 0.52f, colr.y + 0.44f, colr.z + 0.28f, 1.f},
                    (float[]) {colr.x + 0.52f, colr.y + 0.44f, colr.z + 0.28f, 1.f},
                    (float[3]) {0.f},
                    0.4f);

        glNormal3f(0.f, 0.f, -1.f);
        glVertex3f(0.f, 0.f, 0.f); // Centre of triangle fan

        for (int t = 0; t <= res; t++) {
            glVertex3f(radius * sin(t * period), radius * cos(t * period), 0.f);
        }

    glEnd();

    glBegin(GL_TRIANGLE_FAN); // Closer lid

        glNormal3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.f, length);

        for (int t = res; t >= 0; t--) {
            glVertex3f(radius * sin(t * period), radius * cos(t * period), length);
        }

    glEnd();

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

    Vec3f col{}; // Temp holder for getting colours from 'baseLogColours'

    // Push new attribute state for lighting properties
    glPushAttrib(GL_SPECULAR | GL_DIFFUSE | GL_AMBIENT | GL_SHININESS);

    glPushMatrix();

        FloatingModel::doTransform();

        glPushMatrix(); // Push central position of raft, and draw logs making up floor of raft

            // Translate to far NW corner of raft
            glTranslatef(-halfwidth, 0.f, -halflength);

            for (int i = 0; i < numBaseLogs; i++, glTranslatef(baseLogRadius * 2.f + baseLogGap, 0.f, 0.f)) {

                col = browns[baseLogColours[i]];

                glPushMatrix(); // Draw individual logs with slight variation in z direction (given by colour values!)

                    glTranslatef(0.f, 0.f, ((col.x - 0.2f) * 5));

                    drawLog(baseLogRadius + col.z, length + col.x * 4, col);

                glPopMatrix();
            }

        glPopMatrix();

        // Twist mast + attachments so is not exactly perpendicular to base logs
        glRotatef(21, 0.f, 1.f, 0.f);

        glPushMatrix(); // Draw central mast

            glRotatef(270, 1.f, 0.f, 0.f);

            drawLog(baseLogRadius * 0.9f, mastHeight, browns[2]);

        glPopMatrix();


        glPushMatrix(); // Draw lantern arm, i.e. the log that 'holds' the lantern up

            // Translate to just above lantern, and right a bit so it appears to go through mast
            glTranslatef(width * 0.06f, lanternheight * 1.1f, 0.f);
            glRotatef(-90, 0.f, 1.f, 0.f);

            drawLog(baseLogRadius * 0.48f, lanternjut * 1.4f, browns[1]);

        glPopMatrix();


        glPushMatrix();  // Draw lantern arm support

            // Translate to just below arm's position
            glTranslatef(0.f, lanternheight * 0.92f, 0.f);
            glRotatef(-90, 0.f, 1.f, 0.f);
            glRotatef(-47, 1.f, 0.f, 0.f);

            drawLog(baseLogRadius * 0.4f, mastHeight * 0.2f, browns[3]);

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

/** De-point and delete heap objects. */
Raft::~Raft() {

    lantern = nullptr;
    baseLogColours = nullptr;

    delete lantern;
    delete baseLogColours;

}

void Raft::update() {

    FloatingModel::update();
    lantern->update();
}


/** Draws a cuboid with given cross-section height and width, and given length.
 *  Cuboid is drawn updwards (+y), w.r.t. current matrix. */
void drawCuboid(const float width, const float height, const float length, const Vec3f colr) {

    const float halfwidth = width / 2.f;
    const float halfheight = height / 2.f;

    glBegin(GL_QUADS);

        glNormal3f(0.f, -1.f, 0.f); // Bottom face

        glVertex3f(-halfwidth, 0.f, -halfheight);
        glVertex3f(halfwidth, 0.f, -halfheight);
        glVertex3f(halfwidth, 0.f, halfheight);
        glVertex3f(-halfwidth, 0.f, halfheight);

        glNormal3f(-1.f, 0.f, 0.f); // Left face

        glVertex3f(-halfwidth, 0.f, -halfheight);
        glVertex3f(-halfwidth, 0.f, halfheight);
        glVertex3f(-halfwidth, length, halfheight);
        glVertex3f(-halfwidth, length, -halfheight);

        glNormal3f(0.f, 0.f, 1.f); // Front face

        glVertex3f(-halfwidth, 0.f, halfheight);
        glVertex3f(halfwidth, 0.f, halfheight);
        glVertex3f(halfwidth, length, halfheight);
        glVertex3f(-halfwidth, length, halfheight);

        glNormal3f(1.f, 0.f, 0.f); // Right face

        glVertex3f(halfwidth, 0.f, halfheight);
        glVertex3f(halfwidth, 0.f, -halfheight);
        glVertex3f(halfwidth, length, -halfheight);
        glVertex3f(halfwidth, length, halfheight);

        glNormal3f(0.f, 0.f, -1.f); // Back face

        glVertex3f(halfwidth, 0.f, -halfheight);
        glVertex3f(-halfwidth, 0.f, -halfheight);
        glVertex3f(-halfwidth, length, -halfheight);
        glVertex3f(halfwidth, length, -halfheight);

        glNormal3f(0.f, 1.f, 0.f); // Top face

        glVertex3f(-halfwidth, length, -halfheight);
        glVertex3f(halfwidth, length, -halfheight);
        glVertex3f(halfwidth, length, halfheight);
        glVertex3f(-halfwidth, length, halfheight);

    glEnd();

}





























