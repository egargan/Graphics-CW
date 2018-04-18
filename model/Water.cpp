//
// Created by Edward Gargan on 10/04/2018.
//


#include "Water.h"

#include "../perlin.h"
#include "../Utility.h"

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

            // '3' == number of perlin noise samples
            mesh[x][y] = octaveNoise(x, y, time, 3) * tilelength * amplitude;
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

void Water::draw() const {

    const auto gridsize = (int) mesh.size();
    const auto gridwidth = gridsize / 2;

    // Holds 4 points of each mesh tile
    auto points = std::vector<Vec3f>(4);

    Vec3f temp = Vec3f();

    // Push new attribute state for lighting properties
    glPushAttrib(GL_SPECULAR | GL_DIFFUSE | GL_AMBIENT | GL_SHININESS);

    // Prep. material lighting attributes for mesh polys
    materialise((float[]){0.05f, 0.3f, 0.8f, 1.f},       // Ambient colour
                (float[]){0.f, 0.3f, 0.6f, 1.f},     // Diffuse
                (float[]){1.f, 1.f, 1.f, 1.f},
                128.f);    // Specular

    glPushMatrix();

    // Translate to far NW corner of mesh
    glTranslatef(-gridwidth*tilelength, 0.f, -gridwidth*tilelength);

    // Iterate over grid
    for (int x = 0; x < gridsize-1; x++, glTranslatef(tilelength, 0.f, (-gridsize + 1)*tilelength)) {
        for (int z = 0; z < gridsize-1; z++, glTranslatef(0.f, 0.f, tilelength)) {

            // Points populated counter-clockwise from NW point
            points[0].x = 0.f,        points[0].y = mesh[x][z],     points[0].z = 0.f;
            points[1].x = 0.f,        points[1].y = mesh[x][z+1],   points[1].z = tilelength;
            points[2].x = tilelength, points[2].y = mesh[x+1][z+1], points[2].z = tilelength;
            points[3].x = tilelength, points[3].y = mesh[x+1][z],   points[3].z = 0.f;

            glBegin(GL_TRIANGLES);

            temp = cross(points[1]-points[0], points[2]-points[0]);
            temp /= temp.magnitude(); // OpenGL expects unit normal vectors, so we / by magnitude

            glNormal3f(temp.x, temp.y, temp.z);

            glVertex3f(points[0].x, points[0].y, points[0].z);
            glVertex3f(points[1].x, points[1].y, points[1].z);
            glVertex3f(points[2].x, points[2].y, points[2].z);

            temp = cross(points[3]-points[2], points[0]-points[2]);
            temp /= temp.magnitude();

            glNormal3f(temp.x, temp.y, temp.z);

            glVertex3f(points[2].x, points[2].y, points[2].z);
            glVertex3f(points[3].x, points[3].y, points[3].z);
            glVertex3f(points[0].x, points[0].y, points[0].z);

            glEnd();
        }
    }

    glPopAttrib();
    glPopMatrix();

}

Vec3f Water::getSurfaceNormal(const int x1, const int z1,
                              const int x2, const int z2,
                              const int x3, const int z3) const {

    Vec3f temp = cross(Vec3f{(float)x2, mesh[x2][z2], (float)z2} - Vec3f{(float)x1, mesh[x1][z1], (float)z1},
                       Vec3f{(float)x3, mesh[x3][z3], (float)z3} - Vec3f{(float)x1, mesh[x1][z1], (float)z1});

    return temp /= temp.magnitude();

}

float Water::getMeshY(const int x, const int z) const {

    return mesh[x][z];
}


