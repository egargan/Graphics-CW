
#ifndef COURSEWORK_MODEL_H
#define COURSEWORK_MODEL_H

#include <glut/glut.h>
#include "Vec3.h"

class Model {

protected:

    Vec3f location;

public:

    /** Constructs model at given location. */
    explicit Model(Vec3f _location) : location{_location} {};

    /** Default constructor -- assumes location at (0, 0, 0). */
    Model() : location{Vec3f()} {};

    // '= 0' makes pure virtual function, i.e. an abstract class member function
    /** Render model. **/
    virtual void draw() const = 0;

    /** Update model position by a single 'tick'. */
    virtual void update() = 0;

};



#endif //COURSEWORK_MODEL_H
