#ifndef gameController_hpp
#define gameController_hpp

#include <stdio.h>
#include "GObject.hpp"
#include "GShape.hpp"
#include "GStack.hpp"
#include <vector>
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "customObjects.h"

using namespace std;

class GameController {
public:
    vector<GObject*> objects;
    void drawElements();
    void addObject(GObject *obj);
    void detectColisions();
    GStack* player;
    bool playerAlive = false;
    
    void init();
    
    GameController();
    
private:
    VAO* vao;
    VBO* vbo1;
    EBO* ebo1;
    GLfloat *vArray;
    GLuint* indices = NULL;
};

#endif /* gameController_hpp */
