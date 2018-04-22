//
// Created by Edward Gargan on 21/04/2018.
//

#include "Raft.h"
#include "../Utility.h"

Raft::Raft(Water *water, Vec3f _location, float _width, float _length, float height) :
        FloatingModel(water, _location, _width, _length), mastHeight{height} {

    // TODO: maybe size logs according to raft width and height, e.g. few chunky logs if raft is extra long?

    // logs will be approximately this size, adjusted slightly to exactly fit width
    const float idealw = 1.5f;

    baseLogRadius = (idealw + (fmod(width, idealw) / (floor(width / idealw)))) / 2.f;

    numBaseLogs =  (int) std::round(width / baseLogRadius);

}

void Raft::drawCylinder(float radius, float length) const {

    int res = 20; // Number of quads comprising cylinder

    float period = (float) (M_PI * 2) / res;
    float x, z;

    glBegin(GL_TRIANGLE_FAN); // 'Top' lid for cylinder

    glVertex3f(0.f, 0.f, 0.f); // Centre of 'triangle fan'
    glNormal3f(0.f, -1.f, 0.f);

    for (int t = 0; t <= res; t++) {
        glVertex3f(radius * sin(t * period), 0.f, radius * cos(t * period));
    }

    glEnd();


    glBegin(GL_QUAD_STRIP); // Cylinder length

    for (int t = 0; t <= res; t++) {

        x = radius * cos(t * period);
        z = radius * sin(t * period);

        glNormal3f(cos((t+0.5f) * period), 0.f, sin((t+0.5f) * period));

        glVertex3f(x, 0.f, z);
        glVertex3f(x, length, z);
    }

    glEnd();


    glBegin(GL_TRIANGLE_FAN); // Bottom lid

    glVertex3f(0.f, length, 0.f);
    glNormal3f(0.f, 1.f, 0.f);

    for (int t = 0; t <= res; t++) {
        glVertex3f(radius * sin(t * period), length, radius * cos(t * period));
    }

    glEnd();

}

void Raft::draw() const {

    const float halfwidth = width / 2;
    const float halflength = length / 2;

    // Push new attribute state for lighting properties
    glPushAttrib(GL_SPECULAR | GL_DIFFUSE | GL_AMBIENT | GL_SHININESS);

    //float goodbrown[] = {0.31f, 0.24f, 0.19f, 1.f};

    // Prep. material lighting attributes for mesh polys
    materialise((float[]){0.2f, 0.17f, 0.15f, 1.f},     // Ambient colour
                (float[]){0.31f, 0.24f, 0.19f, 1.f},    // Diffuse
                (float[]){0.1f, 0.8f, 0.1f, 1.f},       // Specular
                20.f);                                  // Shininess

    glPushMatrix();

    doTransform();

    // Translate to far NW corner of mesh
    glTranslatef(-halfwidth, 0.f, -halflength);

    for (int i = 0; i < numBaseLogs; i++, glTranslatef(baseLogRadius * 2, 0.f, 0.f)) {

        // TODO: change drawCylinder() to draw logs across x/z

        drawCylinder(baseLogRadius, length);
    }

    glPopMatrix();

    // draw upwards cylinder


    glPopAttrib();

}

