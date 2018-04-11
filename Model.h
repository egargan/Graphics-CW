
#ifndef COURSEWORK_MODEL_H
#define COURSEWORK_MODEL_H

#include <glut/glut.h>

class Model {

protected:

public:

    // '= 0' makes pure virtual function, i.e. an abstract class member function
    //Model();
    //~Model();

    /** Render model. **/
    virtual void draw() = 0;

    /** Update model position by a single 'tick'. */
    virtual void update() = 0;

};

#endif //COURSEWORK_MODEL_H
