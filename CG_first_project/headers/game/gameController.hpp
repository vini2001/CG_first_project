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
#include "GText.hpp"



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
    void drawText(string text, float x, float y, float scale, glm::vec3 colors);
    void destroy();
    
    ~GameController();
    
    GStack* player;
    bool playerAlive = false;
    
    void init();
    
    GameController();
    
private:
    VAO* vao;
    VBO* vbo1;
    EBO* ebo1;
    GLfloat* vArray = NULL;
    GLuint* indices = NULL;
    long lastShot = 0;
    bool shoot = false;
    long changeDirAt = 0;
    bool goingEsq = false;
    GText *gText;
};

#endif /* gameController_hpp */
