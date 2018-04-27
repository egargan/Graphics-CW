
#ifndef COURSEWORK_MODEL_H
#define COURSEWORK_MODEL_H

#include <GLUT/glut.h>
#include "../Vec3.h"

class Model {

protected:

    Vec3f location;

    /** Translates model to given location.
     * Can be overridden to apply custom transformations. */
    inline void doTransform() const { glTranslatef(location.x, location.y, location.z); }

public:

    /** Constructs model at given location. */
    explicit Model(Vec3f _location) : location{_location} {};

    /** Default constructor -- places object at origin. */
    Model() : location{Vec3f()} {};

    // '= 0' makes pure virtual function, i.e. an abstract class member function
    /** Render model. **/
    virtual void draw() const = 0;

    /** Update model position by a single 'tick'. */
    virtual void update() = 0;

};



#endif //COURSEWORK_MODEL_H
