//
// Created by Edward Gargan on 26/04/2018.
//

#include "Lantern.h"
#include "../Utility.h"



/** Draws lantern at current (relative) origin.
 *
 *  Lantern's glass panels are semi-transparent, and reveal a fluctuating sphere inside,
 *  simulating a flickering candle.
 *
 *  Creates light source within lantern also. */
void Lantern::draw() const {

    // Lamp dimensions
    const int panels = 6;                 // number of glass panels in lamp
    const float radius = 1.2f;
    const float totalheight = 3.f;

    const int strutwr = 3;

    Vec3f cagecolr = {0.22f, 0.18f, 0.18f};

    glPushAttrib(GL_SPECULAR | GL_DIFFUSE | GL_AMBIENT | GL_SHININESS);

    materialise((float[]){cagecolr.x, cagecolr.y, cagecolr.z, 1.f},
                (float[]){cagecolr.x, cagecolr.y, cagecolr.z, 1.f},
                (float[]){cagecolr.x - .2f, cagecolr.y - .2f, cagecolr.z - .2f, 1.f},
                1.f);

    // TODO: clean up -- look at simplifying for loops, e.g. adding to t avoid unnecessary iterations?

    glBegin(GL_TRIANGLE_FAN); // Lamp roof

    const float roofheight = totalheight / 4.f;
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


    glBegin(GL_TRIANGLE_FAN); // Upward-facing bottom

    glNormal3f(0.f, 1.f, 0.f);
    glVertex3f(0.f, -totalheight, 0.f);

    for (int t = 0; t <= panels * strutwr * 2 + 1; t++) {

        if (t % (strutwr * 2) < 2) {
            glVertex3f(radius * sin(t * period), -totalheight, radius * cos(t * period));
        };
    }

    glEnd();


    glBegin(GL_TRIANGLE_FAN); // Downward-facing bottom

    glNormal3f(0.f, -1.f, 0.f);
    glVertex3f(0.f, -totalheight, 0.f);

    for (int t = panels * strutwr * 2 + 1; t >= 0; t--) {

        if (t % (strutwr * 2) < 2) {
            glVertex3f(radius * sin(t * period), -totalheight, radius * cos(t * period));
        };
    }

    glEnd();


    // Draw candle + make light source
    glPushMatrix();

        glTranslatef(0.f, -totalheight * 0.7f, 0.f);

        makeLight(GL_LIGHT2,
                  (float[]) {0.f, -1.f, 0.f},                           // direction
                  (float[]) {0.f, 0.f, 0.f, 1.f},                       // position
                  (float[]) {0.f, 0.f, 0.f},                            // ambient light
                  (float[]) {255 / 255.f, 180 / 255.f, 74 / 255.f},     // diffuse
                  (float[]) {247 / 255.f, 187 / 255.f, 74 / 255.f},     // specular
                  180.f,                                                // cutoff
                  1.f);                                                // constant attenuation

        // Give light special attenuation so we're not lighting the whole scene up
        glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION,    0.02f - lightVary[lightVaryIter] * 0.5f);
        glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.005f);

        glDisable(GL_LIGHTING);

        glColor4f(1.f, 0.8f, 0.5f, 0.7f);
        glutSolidSphere(0.3f + lightVary[lightVaryIter], 6, 6);

        glEnable(GL_LIGHTING);

    glPopMatrix();



    glBegin(GL_QUAD_STRIP); // Draw panels and cage struts

    float x, z;

    for (int t = 0; t <= panels * strutwr * 2; t++) {

        if (t % (strutwr * 2) < 2) {

            // Slightly indent lamp cage from roof
            x = radius * 0.9f * sin(t * period);
            z = radius * 0.9f * cos(t * period);

            glVertex3f(x, -roofheight, z);
            glVertex3f(x, -totalheight, z);

            // Normal + material properties applied to *next* set of vertices
            glNormal3f(sin(t * period), 0.f, cos(t * period));

            materialise((float[]){cagecolr.x, cagecolr.y, cagecolr.z, 1.f},
                        (float[]){cagecolr.x, cagecolr.y, cagecolr.z, 1.f},
                        (float[]){0.3f, 0.3f, 0.3f, 1.f},
                        1.f);

        } else if (t % strutwr == 0) {

            // Define normal + material properties of lamp 'windows'
            glNormal3f(sin(t * period), 0.f, cos(t * period));

            materialise((float[]){0.6f, 0.61f, 0.6f, .2f},
                        (float[]){0.6f, 0.61f, 0.6f, .2f},
                        (float[]){0.6f, 0.61f, 0.6f, .2f},
                        1.f);

        }
    }

    glEnd();

    glPopAttrib();

}

/** Updates lightVary iter for flickering light source and candle animation. */
void Lantern::update() {

    // Ensure light vary iter remains within bounds of lightVary array.
    if (++lightVaryIter >= lightVary.size()) lightVaryIter = 0;

}
