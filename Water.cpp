//
// Created by Edward Gargan on 10/04/2018.
//

#include "Water.h"
#include "perlin.h"

#include <iostream>
#include <limits>


Water::Water(int size) {

    wavegrid = std::vector<std::vector<float>>((size_t)size, std::vector<float>((size_t)size, 0));

}

Water::~Water() {

    wavegrid.clear();

}

float z = 0.f;

void Water::update() {

    const auto gridsize = (int) wavegrid.size();

    // Iterate over grid
    for (int x = 0; x < gridsize; x++) {
        for (int y = 0; y < gridsize; y++) {

            wavegrid[x][y] = octaveNoise(x, y, z, 3)*0.05f;
        }
    }

    // Check that z doesn't exceed float limit, in case we leave our program for *a while*.
    // Finding an interval for z, e.g. resetting every z == 5, would create periodicity in water animation -- bad!
    if (z < std::numeric_limits<float>::max()) {
        z += 0.003f;
    } else {
        z = 0.f;
    }


}

void Water::draw() {

    const auto gridsize = (int) wavegrid.size();
    const auto gridwidth = gridsize / 2;

    glTranslatef(-gridwidth * 0.1f, 0.f, -gridwidth*0.1f);

    // Iterate over grid
    for (int x = 0; x < gridsize-1; x++) {
        for (int z = 0; z < gridsize-1; z++) {

            glBegin(GL_TRIANGLES);

            glColor3f(0.f, 0.1f, 0.8f);

            glVertex3f(0.f,  wavegrid[x][z]    , 0.f);
            glVertex3f(0.f,  wavegrid[x][z+1]  , 0.1f);
            glVertex3f(0.1f, wavegrid[x+1][z+1], 0.1f);

            glColor3f(0.f, 0.05f, 0.7f);

            glVertex3f(0.1f, wavegrid[x+1][z+1], 0.1f);
            glVertex3f(0.1f, wavegrid[x+1][z], 0.f);
            glVertex3f(0.f,  wavegrid[x][z], 0.f);

            glEnd();
//
            glTranslatef(0.f, 0.f, 0.1f);

        }
        glTranslatef(0.1f, 0.f, -gridsize * 0.1f + 0.1f);
    }



}


