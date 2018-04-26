//
// Created by Edward Gargan on 21/04/2018.
//

#include "Raft.h"
#include "../Utility.h"

Raft::Raft(Water *water, const Vec3f _location, const float _width, const float _length, const float height) :
        FloatingModel(water, _location, _width, _length), mastHeight{height} {

    // TODO: maybe size logs according to raft width and height, e.g. fewer chunky logs if raft is extra long?

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
    materialise((float[]) {colr.x + 0.35f, colr.y + 0.25f, colr.z + 0.1f, 1.f},
                (float[]) {colr.x + 0.55f, colr.y + 0.45f, colr.z + 0.3f, 1.f},
                nullptr,
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


void Raft::draw() const {

    const float halfwidth = width / 2;
    const float halflength = length / 2;

    // Push new attribute state for lighting properties
    glPushAttrib(GL_SPECULAR | GL_DIFFUSE | GL_AMBIENT | GL_SHININESS);
    glPushMatrix();

    doTransform();

    glPushMatrix(); // Push central position of raft

        // Translate to far NW corner of mesh
        glTranslatef(-halfwidth, 0.f, -halflength);

        // TODO: add a bit of fixed random variation to log length + position + radius (further than just adding color vals!)

        for (int i = 0; i < numBaseLogs; i++, glTranslatef(baseLogRadius * 2.f + baseLogGap, 0.f, 0.f)) {

            Vec3f col = browns[baseLogColours[i]];

            glPushMatrix();

                glTranslatef(0.f, 0.f, ((col.x - 0.2f) * 5)); // Add slight random value to size and Z

                drawLog(baseLogRadius + col.z, length + col.x * 4, col);

            glPopMatrix();
        }

    glPopMatrix();


    glPushMatrix();

        glRotatef(-91, 1.f, 0.f, 0.f);

        drawLog(baseLogRadius - baseLogRadius / 10.f, mastHeight, browns[baseLogColours[1]]);

    glPopMatrix();

    glTranslatef(-3.f, mastHeight * 0.6f, 0.f);

    lantern->draw();


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





























