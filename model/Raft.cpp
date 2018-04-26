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

    // baseLogRadius --

    // printf("width: %f, op: %f", width, (numBaseLogs * baseLogRadius * 2) + (loggap * (numBaseLogs - 1)));

    // Populate log colours array, one colour per log

    baseLogColours = new int[numBaseLogs] {0};

    srand(30);

    for (int i = 0; i < numBaseLogs; i++) {
        baseLogColours[i] = rand() % (int) browns.size();
    }

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

float rot = 0.f;

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

    //9glRotatef(rot++, 1.f, 0.f, 0.f);

    drawLamp();


    glPopMatrix();
    glPopAttrib();

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

void Raft::drawLamp() const {

    // Push new attribute state for lighting properties
    glPushAttrib(GL_SPECULAR | GL_DIFFUSE | GL_AMBIENT | GL_SHININESS);

    materialise((float[]){browns[2].x, browns[2].y, browns[2].z, 1.f},
                (float[]){browns[2].x, browns[2].y, browns[2].z, 1.f},
                (float[]){browns[2].x + .2f, browns[2].y + .2f, browns[2].z + .2f, 1.f},
                1.f);

    // Lamp dimensions
    const int panels = 6; // number of glass panels in lamp
    const float radius = 1.6f;
    const float totalheight = 4.f;

    const int strutwr = 4;

    // Draw lamp downwards from current position, i.e. peak of lamp is drawn at relative origin

    // TODO: clean up -- look at simplifying for loops, e.g. adding to t t avoid unnecessary iterations?

    glBegin(GL_TRIANGLE_FAN); // Lamp roof

        const float roofheight = totalheight / 5.f;
        constexpr float period = (float) M_PI * 2 / (panels * strutwr * 2);

        glVertex3f(0.f, 0.f, 0.f); // Centre of triangle fan

        Vec3f norm{};

        for (int t = 0; t <= panels * strutwr * 2 + 1; t++) {

            if (t % (strutwr * 2) < 2) {

                glVertex3f(radius * sin(t * period), -roofheight, radius * cos(t * period));

                //norm = {(-roofheight  *  radius * cos((t+1) * period))  -  (radius * cos(t * period)  *  -roofheight),
                //       (radius * cos(t * period)  *  radius * sin((t+1) * period))  -  (radius * sin(t * period)  *  radius * cos((t+1) * period)),
                //       (radius * sin(t * period)  *  -roofheight)  -  (-roofheight  *  radius * sin((t+1) * period))};

                norm = {roofheight * radius * (cos(period * t) - cos(period * (t+1))),
                        radius * radius * sin(period),
                        roofheight * radius * (sin(period * (t+1)) - sin(period * t))};

                norm /= norm.magnitude();

                glNormal3f(norm.x, norm.y, norm.z);

            } else if (t % strutwr == 0) {

                //norm = {(-roofheight  *  radius * cos((t+1) * period))  -  (radius * cos((t-1) * period)  *  -roofheight),
                //        (radius * cos((t-1) * period)  *  radius * sin((t+1) * period))  -  (radius * sin((t-1) * period)  *  radius * cos((t+1) * period)),
                //        (radius * sin((t-1) * period)  *  -roofheight)  -  (-roofheight  *  radius * sin((t+1) * period))};

                norm = {2 * roofheight * radius * sin(period) * sin(period * t),
                        radius * radius * sin(2 * period),
                        2 * roofheight * radius * sin(period) * cos(period * t)};

                norm /= norm.magnitude();

                glNormal3f(norm.x, norm.y, norm.z);
            };

        }

    glEnd();


    glBegin(GL_QUAD_STRIP);

        float x, z;

        for (int t = 0; t <= panels * strutwr * 2 + 1; t++) {

            if (t % (strutwr * 2) < 2) {

                // Slightly indent lamp cage from roof
                x = radius * 0.9f * sin(t * period);
                z = radius * 0.9f * cos(t * period);

                glVertex3f(x, -roofheight, z);
                glVertex3f(x, -totalheight, z);

                // Normal + material properties applied to *next* set of vertices
                glNormal3f(sin(t * period), 0.f, cos(t * period));

                materialise((float[]){browns[2].x, browns[2].y, browns[2].z, 1.f},
                            (float[]){browns[2].x, browns[2].y, browns[2].z, 1.f},
                            (float[]){0.3f, 0.3f, 0.3f, 1.f},
                            1.f);

            } else if (t % strutwr == 0) {

                glNormal3f(sin(t * period), 0.f, cos(t * period));

                materialise((float[]){0.7f, 0.6f, 0.2f, 1.f},
                            (float[]){0.7f, 0.6f, 0.2f, 1.f},
                            (float[]){0.3f, 0.3f, 0.3f, 1.f},
                            1.f);

            }

        }

    glEnd();


    glBegin(GL_TRIANGLE_FAN); // Closer lid

        glNormal3f(0.f, -1.f, 0.f);
        glVertex3f(0.f, -totalheight, 0.f);

        for (int t = panels * strutwr * 2 + 1; t >= 0; t--) {

            if (t % (strutwr * 2) < 2) {
                glVertex3f(radius * sin(t * period), -totalheight, radius * cos(t * period));
            };
        }

    glEnd();

    //drawCuboid(5, 5, 10, {0.2f, 0.4f, 0.4f});

    glPopAttrib();

}






























