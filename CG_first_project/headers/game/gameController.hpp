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
    void handleInput(GLuint pressedKey, GLuint pressedMouseButton);
    void frameActions();
    void fire(GStack *spaceShip);
    void destroy();
    
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
    long lastShot = 0;
    bool shoot = false;
    long changeDirAt = 0;
    bool goingEsq = false;
};

#endif /* gameController_hpp */
