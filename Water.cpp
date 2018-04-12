//
// Created by Edward Gargan on 10/04/2018.
//

#include "Water.h"
#include "perlin.h"

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

            mesh[x][y] = (float)(octaveNoise(x, y, time, 2) * (tilelength * 0.5));
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

    glTranslatef(-gridwidth*tilelength, 0.f, -gridwidth*tilelength);

    // Iterate over grid
    for (int x = 0; x < gridsize-1; x++, glTranslatef(tilelength, 0.f, (-gridsize + 1)*tilelength)) {
        for (int z = 0; z < gridsize-1; z++, glTranslatef(0.f, 0.f, tilelength)) {

            glBegin(GL_TRIANGLES);

            glColor3f(0.f, 0.1f, 0.8f);

            glVertex3f(0.f,  mesh[x][z]    , 0.f);
            glVertex3f(0.f,  mesh[x][z+1]  , tilelength);
            glVertex3f(tilelength, mesh[x+1][z+1], tilelength);

            glColor3f(0.f, 0.05f, 0.7f);

            glVertex3f(tilelength, mesh[x+1][z+1], tilelength);
            glVertex3f(tilelength, mesh[x+1][z], 0.f);
            glVertex3f(0.f,  mesh[x][z], 0.f);

            glEnd();

        }
    }

}


