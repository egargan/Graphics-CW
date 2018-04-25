//
// Created by Edward Gargan on 18/04/2018.
//

#include "Utility.h"

/** Calls gl methods to setup a surface that reflects light -- make sure to push + pop attribs either side!  */
void materialise(float amb[], float dif[], float spec[], float shine)  {

    // Allow null values
    if (amb != nullptr)     glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    if (dif != nullptr)     glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
    if (spec != nullptr)    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    if (shine != 0.0)       glMaterialf(GL_FRONT, GL_SHININESS, shine);

}

int loadBMP(const std::string& path) {

    // Data read from the header of the BMP file
    char header[54];     // BMP files have a standard 56-byte header

    int width, height;
    int imageSize;
    int colorDepth;


    FILE *file = fopen(path.c_str(), "rb");

    if (!file) {
        printf("Image could not be opened\n");
        return 0;
    }

    // If 53 bytes not readable, or first two bytes are not 'B' and 'M', we don't have a BMP
    if (fread(header, 1, 54, file) != 54 || header[0] != 'B' || header[1] != 'M') {
        printf("Not a correct BMP file\n");
        return 0;
    }


    // Read ints from the byte array
    width =         *(int *) &(header[0x12]);
    height =      abs((int) *&(header[0x16])); //...

    // BMPs are usually stored from bottom to top. When the height in the bitmap header is negative
    // then it means that the image is stored from top to bottom, and its real height is abs(height).

    // To display the image correctly, starting from the bottom if the height is positive
    // or from the top if the height is negative).

    imageSize =     *(int *) &(header[0x22]);
    colorDepth =    *(int *) &(header[0x1C]); // bits per pixel -- /8 for byte value

    // Image size can be given as 0 as per standard, when image is uncompressed (?)
    if (imageSize == 0) imageSize = width * height * (colorDepth / 8); // byte per RGB

    // Create a buffer
    auto data = new unsigned char[imageSize];

    //printf("width %i, height %i, imageSize %i, colord %i\n", width, height, imageSize, colorDepth);

    // Read file to buffer
    fread(data, 1, (size_t) imageSize, file);

    fclose(file);


    glEnable(GL_TEXTURE_2D);

    unsigned int texObject;

    // Generate a texture buffer
    glGenTextures(1, &texObject);

    // Bind to texture buffer, so we can apply attributes
    glBindTexture(GL_TEXTURE_2D, texObject);

    // Apply texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Read image data to bound texture
    // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
    glTexImage2D(GL_TEXTURE_2D, static_cast<GLint>(0), colorDepth == 32 ? GL_RGBA : GL_RGB,
                 static_cast<GLsizei>(width),
                 static_cast<GLsizei>(height),
                 static_cast<GLint>(0),
                 colorDepth == 32 ? GL_BGRA : GL_BGR_EXT,
                 GL_UNSIGNED_BYTE,
                 static_cast<GLvoid *>(data));

    glDisable(GL_TEXTURE_2D);

    // Now that image data is bound to texture ID, we're done with data buffer
    delete[] data;

    return texObject;
}

