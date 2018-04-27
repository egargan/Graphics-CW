//
// Created by Edward Gargan on 17/04/2018.
//

#include "Crate.h"

/** Constructs model as a 'FloatingModel' and loads crate texture. */
Crate::Crate(Water *water, Vec3f _location, float _width) :
        FloatingModel(water, _location, _width, _width) {

    texId = (unsigned int) loadBMP("../textures/crate/plank_oak.bmp");
}

/** Draws crate with texture given by constructor. */
void Crate::draw() const {

    const float halfwidth = width / 2.f;

    glPushMatrix();

        glPushAttrib(GL_ALL_ATTRIB_BITS);

        // Transform to crate location + rotate
        FloatingModel::doTransform();

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texId);

        // 'GL_MODULATE' multiples light color by texture color
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        // Give material lighting properties that combine with texture
        materialise((float[]){0.5f, 0.5f, 0.5f, 1.f},
                    (float[]){0.7f, 0.7f, 0.7f, 1.f},
                    (float[]){0.3f, 0.3f, 0.3f, 1.f},
                    1.f);

        glBegin(GL_QUADS);

            // Front face
            glNormal3f(0.0f, 0.0f, 1.0f);

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-halfwidth, halfwidth, halfwidth);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-halfwidth, -halfwidth, halfwidth);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(halfwidth, -halfwidth, halfwidth);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(halfwidth, halfwidth, halfwidth);

            // Back face
            glNormal3f(0.0f, 0.0f, -1.0f);

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(halfwidth, halfwidth, -halfwidth);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(halfwidth, -halfwidth, -halfwidth);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(-halfwidth, -halfwidth, -halfwidth);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(-halfwidth, halfwidth, -halfwidth);

            // Left face
            glNormal3f(-1.0f, 0.0f, 0.0f);

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-halfwidth, halfwidth, -halfwidth);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-halfwidth, -halfwidth, -halfwidth);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(-halfwidth, -halfwidth, halfwidth);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(-halfwidth, halfwidth, halfwidth);

            // Right face
            glNormal3f(1.0f, 0.0f, 0.0f);

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(halfwidth, halfwidth, halfwidth);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(halfwidth, -halfwidth, halfwidth);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(halfwidth, -halfwidth, -halfwidth);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(halfwidth, halfwidth, -halfwidth);

            // Top face
            glNormal3f(0.0f, 1.0f, 0.0f);

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-halfwidth, halfwidth, -halfwidth);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-halfwidth, halfwidth, halfwidth);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(halfwidth, halfwidth, halfwidth);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(halfwidth, halfwidth, -halfwidth);

            // Bottom face
            glNormal3f(0.0f, -1.0f, 0.0f);

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f  (-halfwidth, -halfwidth, halfwidth);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-halfwidth, -halfwidth, -halfwidth);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(halfwidth, -halfwidth, -halfwidth);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(halfwidth, -halfwidth, halfwidth);

        glEnd();


        // Bind to the NULL texture buffer
        glBindTexture(GL_TEXTURE_2D, (GLuint) NULL);

        glDisable(GL_TEXTURE_2D);

        glPopAttrib();

    glPopMatrix();

}



