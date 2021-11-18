#ifndef GStar_hpp
#define GStar_hpp

#include <stdio.h>

#include <stdio.h>
#include "GShape.hpp"

class GStar : public GShape {
public:
    GStar(GLfloat x, GLfloat y);
    bool update();
    
    int row, pos;
};


#endif /* GStar_hpp */
