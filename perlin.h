//
// Created by Edward Gargan on 10/04/2018.
//

#ifndef GRAPHICS_CW_PERLIN_H
#define GRAPHICS_CW_PERLIN_H

#include "Utility.h"

    // Perlin noise algorithm adapted from,
    // http://staffwww.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf#page=8

    // Permutation array used in hash function
    const int perm[] = {151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233,
                     7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
                     190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219,
                     203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174,
                     20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27,
                     166, 77, 146, 158, 31, 83, 111, 229, 122, 60, 211, 133, 230,
                     220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25,
                     63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169,
                     200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173,
                     186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118,
                     126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
                     189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163,
                     70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19,
                     98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246,
                     97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162,
                     241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181,
                     199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150,
                     254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128,
                     195, 78, 66, 215, 61, 156, 180, 151, 160, 137, 91, 90, 15, 131,
                     13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69,
                     142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75,
                     0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
                     88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74,
                     165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229,
                     122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
                     102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132,
                     187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86,
                     164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124,
                     123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59,
                     227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119,
                     248, 152, 2, 44, 154, 161, 70, 221, 153, 101, 155, 167, 43, 172,
                     9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178,
                     185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210,
                     144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239,
                     107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176,
                     115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114,
                     67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180};

    const int grad3[][3] = {{1,  1,  0}, {-1, 1,  0}, {1,  -1, 0},  {-1, -1, 0},
                            {1,  0,  1}, {-1, 0,  1}, {1,  0,  -1}, {-1, 0,  -1},
                            {0,  1,  1}, {0,  -1, 1}, {0,  1,  -1}, {0,  -1, -1}};

    int fastfloor(float x) {
        return x > 0 ? (int) x : (int) x - 1;
    }


    inline float dot(const int* g, float x, float y, float z) {
        return g[0] * x + g[1] * y + g[2] * z;
    }

    inline float fade(float t) {

        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    float noise(float x, float y, float z) {

        // Find unit grid cell containing point
        int X = fastfloor(x);
        int Y = fastfloor(y);
        int Z = fastfloor(z);

        // Get relative xyz coordinates of point within that cell
        x = x - X;
        y = y - Y;
        z = z - Z;

        // Wrap the integer cells at 255 (smaller integer period can be introduced here)
        X = X & 255;
        Y = Y & 255;
        Z = Z & 255;

        // Calculate a set of eight hashed gradient indices
        int gi000 = perm[X + perm[Y + perm[Z]]] % 12;
        int gi001 = perm[X + perm[Y + perm[Z + 1]]] % 12;
        int gi010 = perm[X + perm[Y + 1 + perm[Z]]] % 12;
        int gi011 = perm[X + perm[Y + 1 + perm[Z + 1]]] % 12;
        int gi100 = perm[X + 1 + perm[Y + perm[Z]]] % 12;
        int gi101 = perm[X + 1 + perm[Y + perm[Z + 1]]] % 12;
        int gi110 = perm[X + 1 + perm[Y + 1 + perm[Z]]] % 12;
        int gi111 = perm[X + 1 + perm[Y + 1 + perm[Z + 1]]] % 12;

        // Calculate noise contributions from each of the eight corners
        float n000 = dot(grad3[gi000], x, y, z);
        float n100 = dot(grad3[gi100], x - 1, y, z);
        float n010 = dot(grad3[gi010], x, y - 1, z);
        float n110 = dot(grad3[gi110], x - 1, y - 1, z);
        float n001 = dot(grad3[gi001], x, y, z - 1);
        float n101 = dot(grad3[gi101], x - 1, y, z - 1);
        float n011 = dot(grad3[gi011], x, y - 1, z - 1);
        float n111 = dot(grad3[gi111], x - 1, y - 1, z - 1);

        // Compute the fade curve value for each of x, y, z
        float u = fade(x);
        float v = fade(y);
        float w = fade(z);

        // Interpolate along x the contributions from each of the corners
        float nx00 = lerp(n000, n100, u);
        float nx01 = lerp(n001, n101, u);
        float nx10 = lerp(n010, n110, u);
        float nx11 = lerp(n011, n111, u);

        // Interpolate the four results along y
        float nxy0 = lerp(nx00, nx10, v);
        float nxy1 = lerp(nx01, nx11, v);

        // Interpolate the two last results along z
        float nxyz = lerp(nxy0, nxy1, w);

        return nxyz;
    }


/** Repeatedly samples noise function with different parameters for less cyclic noise. */
float octaveNoise(float x, float y, float z, int octaves) {

    float total = 0;
    float frequency = 1;

    int i;

    for(i = 0; i<octaves; i++) {

        total += noise(x * frequency, y * frequency, z * frequency);

        // Awkward frequency increase greatly delays animation cycles, i.e. 'periods' in oscillation patterns
        frequency += 1.337f;
    }

    return total / i; // Normalise total value
}



#endif //GRAPHICS_CW_PERLIN_H
