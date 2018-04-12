//
// Created by Edward Gargan on 10/04/2018.
//

#include "Water.h"
#include "Vec3.h"
#include "perlin.h"

#include "Utility.h"

#include <iostream>


/**
 * Constructor -- creates water mesh of 'n' unit tiles.
 * @param meshlength    Mesh side length.
 * */
Water::Water(int meshlength) : Water(1.f, meshlength) {

    // Delegated constructors can only be called in initialization list ^.

}

/**
 * @param tilesize      Side length of individual mesh cell / tile.
 * @param meshlength    Number of tiles across one length of square, i.e. meshsize * tilesize = length of mesh.
 */
Water::Water(float _tilelength, int meshlength) : tilelength{_tilelength} {

    mesh = std::vector<std::vector<float>>((size_t)meshlength, std::vector<float>((size_t)meshlength, 0));

}

Water::~Water() {

    mesh.clear();

}

void Water::update() {

    const auto gridsize = (int) mesh.size();

    // Iterate over grid
    for (int x = 0; x < gridsize; x++) {
        for (int y = 0; y < gridsize; y++) {

            // '2' == number of perlin noise samples
            mesh[x][y] = octaveNoise(x, y, time, 2) * tilelength * amplitude;
        }
    }

    // Check that z doesn't exceed float limit, in case we leave our program for *a while*.
    // Finding an interval for z, e.g. resetting every z == 1, would create periodicity in water animation -- bad!
    if (time < std::numeric_limits<float>::max() - 1.f) {
        time += dtime;
    } else {
        time = 0.f;
    }


}

void Water::draw() {

    const auto gridsize = (int) mesh.size();
    const auto gridwidth = gridsize / 2;

    // Holds 4 points of each mesh tile
    auto points = std::vector<Vec3f>(4);

    Vec3f temp = Vec3f();

    // Translate to far NW corner of mesh
    glTranslatef(-gridwidth*tilelength, 0.f, -gridwidth*tilelength);

    glPushAttrib(GL_SPECULAR & GL_DIFFUSE & GL_AMBIENT & GL_SHININESS);

    materialise((float[]){0.f, 0.2f, 1.f, 1.f},  // Ambient
                (float[]){1.f, 1.f, 1.f, 1.f},   // Diffuse
                (float[]){1.f, 1.f, 1.f, 1.f});  // Specular

    // Iterate over grid
    for (int x = 0; x < gridsize-1; x++, glTranslatef(tilelength, 0.f, (-gridsize + 1)*tilelength)) {
        for (int z = 0; z < gridsize-1; z++, glTranslatef(0.f, 0.f, tilelength)) {

            // Points populated counter-clockwise from NW point
            points[0].x = 0.f,        points[0].y = mesh[x][z],     points[0].z = 0.f;
            points[1].x = 0.f,        points[1].y = mesh[x][z+1],   points[1].z = tilelength;
            points[2].x = tilelength, points[2].y = mesh[x+1][z+1], points[2].z = tilelength;
            points[3].x = tilelength, points[3].y = mesh[x+1][z],   points[3].z = 0.f;

            glBegin(GL_TRIANGLES);

            glColor3f(0.f, 0.1f, 0.8f);

            temp = cross(points[1]-points[0], points[2]-points[0]);
            glNormal3f(temp.x, temp.y, temp.z);

            glVertex3f(points[0].x, points[0].y, points[0].z);
            glVertex3f(points[1].x, points[1].y, points[1].z);
            glVertex3f(points[2].x, points[2].y, points[2].z);

            temp = cross(points[3]-points[2], points[3]-points[2]);
            glNormal3f(temp.x, temp.y, temp.z);

            glColor3f(0.f, 0.05f, 0.7f);

            glVertex3f(points[2].x, points[2].y, points[2].z);
            glVertex3f(points[3].x, points[3].y, points[3].z);
            glVertex3f(points[0].x, points[0].y, points[0].z);

            glEnd();

        }
    }

    glPopAttrib();

}


