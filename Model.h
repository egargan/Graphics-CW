
#ifndef COURSEWORK_MODEL_H
#define COURSEWORK_MODEL_H

#include <vector>

class Model {

protected:

public:

    // '= 0' makes pure virtual function, i.e. an abstract class member function

    virtual void Model() = 0;
    virtual void ~Model() = 0;

    /** Render model. **/
    virtual void display() = 0;

    /** Update model position by a single 'tick'. */
    virtual void update() = 0;

};

#endif //COURSEWORK_MODEL_H
