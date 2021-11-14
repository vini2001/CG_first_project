#include "GStack.hpp"
#include "vectors.hpp"

#ifndef customObjects_h
#define customObjects_h
    GShape* createSquare(GLfloat x, GLfloat y, GLfloat size);
    GShape* createRectangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height);
    GShape* createTriangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height);
    GShape* createTriangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height, bool upsideDown);
    GShape* createRectTriangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height);
    GStack* createSpaceShip(GLfloat x, GLfloat y, Vec2 scale);
    GShape* createAlien(GLfloat x, GLfloat y);
    GStack* createBullet(GLfloat x, GLfloat y);
    void drawSquare(GShape *obj, GVertice v1, GVertice v2, GVertice v3, GVertice v4);
    void drawTriangle(GShape *obj, GVertice v1, GVertice v2, GVertice v3);
#endif
