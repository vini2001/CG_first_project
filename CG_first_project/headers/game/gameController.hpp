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
#include "GAlienSpaceship.hpp"



using namespace std;


struct FlashMessage {
    float x, y;
    string text;
    glm::vec3 rgb;
    long finishAt;
    float scale;
    
    FlashMessage(string text, float x, float y, glm::vec3 rgb, long finishAt) : text(text), x(x), y(y), rgb(rgb), finishAt(finishAt), scale(0.6) {}
    
    FlashMessage(string text, float x, float y, glm::vec3 rgb, long finishAt, float scale) : text(text), x(x), y(y), rgb(rgb), finishAt(finishAt), scale(scale) {}
};

class GameController {
public:
    vector<GObject*> objects;
    void drawElements();
    void addObject(GObject *obj);
    void detectColisions();
    void handleInput(GLuint pressedKey, GLuint pressedMouseButton, Vec2 mousePos);
    void frameActions();
    void fire(GStack *spaceShip);
    void drawText(string text, float x, float y, float scale, glm::vec3 colors);
    void destroy();
    
    ~GameController();
    
    GStack* player;
    bool playerAlive = false;
    
    void init(Shader *shaderProgram);
    
    GameController();
    
private:
    VAO* vao = NULL;
    VBO* vbo1;
    EBO* ebo1;
    GLfloat* vArray = NULL;
    GLuint* indices = NULL;
    long lastShot = 0;
    bool shoot = false;
    long changeDirAt = 0;
    bool goingEsq = false;
    GText *gText;
    
    int playerBulletsLevel = 1;
    float bulletsSpeed = 5.0;
    int shootingInterval = 300;
    
    vector<FlashMessage> flashMessages;
    Shader *shader;
    
    bool compareAlien(GAlien a, GAlien b);
    vector<GAlien*> aliens;
    
    void deleteFromAliensArray(GObject *alienObj);
    void deleteObjectFromObjects(GObject *object);
    
    long lastAttack = 0;
};

#endif /* gameController_hpp */
