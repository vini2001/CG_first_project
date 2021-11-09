#include <stdio.h>
#include "GShape.hpp"
#include "customObjects.h"
#include "vectors.hpp"


GShape* createSquare(GLfloat x, GLfloat y, GLfloat size) {
    GShape *obj = new GShape(x, y);
    GLfloat r = 1.0f, g = 1.0f, b = 1.0f;
    GVertice v1 = GVertice(0.00f, 0.00f, r, g, b);
    GVertice v2 = GVertice(size, 0.00f, r, g, b);
    GVertice v3 = GVertice(size, size, r, g, b);
    GVertice v4 = GVertice(0.00f, size, r, g, b);
    obj->addVertice(v1);
    obj->addVertice(v2);
    obj->addVertice(v3);
    obj->addVertice(v4);
    obj->addTriangle(v1, v2, v3);
    obj->addTriangle(v1, v3, v4);
    return obj;
}

GShape* createRectangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
    GShape *obj = new GShape(x, y);
    GLfloat r = 1.0f, g = 1.0f, b = 1.0f;
    GVertice v1 = GVertice(0.00f, 0.00f, r, g, b);
    GVertice v2 = GVertice(width, 0.00f, r, g, b);
    GVertice v3 = GVertice(width, height, r, g, b);
    GVertice v4 = GVertice(0.00f, height, r, g, b);
    obj->addVertice(v1);
    obj->addVertice(v2);
    obj->addVertice(v3);
    obj->addVertice(v4);
    obj->addTriangle(v1, v2, v3);
    obj->addTriangle(v1, v3, v4);
    return obj;
}

GShape* createTriangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
    return createTriangle(x, y, width, height, false);
}

GShape* createTriangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height, bool upsideDown) {
    GShape *obj = new GShape(x, y);
    GLfloat r = 1.0f, g = 1.0f, b = 1.0f;
    GVertice v1 = GVertice(0.00f, 0.00f, r, g, b);
    GVertice v2 = GVertice(width, 0.00f, r, g, b);
    GVertice v3 = GVertice(width/2, upsideDown ? -height : height, r, g, b);
    obj->addVertice(v1);
    obj->addVertice(v2);
    obj->addVertice(v3);
    obj->addTriangle(v1, v2, v3);
    
    obj->setItemSpawnPos(v3.x, v3.y);
    return obj;
}

GShape* createRectTriangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
    GShape *obj = new GShape(x, y);
    GLfloat r = 1.0f, g = 1.0f, b = 1.0f;
    GVertice v1 = GVertice(0.00f, 0.00f, r, g, b);
    GVertice v2 = GVertice(width, 0.00f, r, g, b);
    GVertice v3 = GVertice(width, height, r, g, b);
    obj->addVertice(v1);
    obj->addVertice(v2);
    obj->addVertice(v3);
    obj->addTriangle(v1, v2, v3);
    return obj;
}

GStack* createAlien(GLfloat x, GLfloat y, Vec2 scale) {
    
    GStack *stR = new GStack(x, y+5);
    
    GShape *square = createSquare(0, 0, 50);
    GShape *eye1 = createSquare(35, 0, 10);
    eye1->rgb(0.09, 0.13, 0.43);
    GShape *eye2 = createSquare(5, 0, 10);
    eye2->rgb(0.09, 0.13, 0.43);
    
    GShape *tri1 = createTriangle(-15, 35, 30, -20);
    tri1->rgb(0.09, 0.13, 0.43);
    GShape *tri2 = createTriangle(35, 35, 30, -20);
    tri2->rgb(0.09, 0.13, 0.43);
    
    GShape *rec1 = createRectangle(-10, 50, 70, 5);
    rec1->rgb(0.09, 0.13, 0.43);
    
    
    stR->addObject(square);
    stR->addObject(eye1);
    stR->addObject(eye2);
    stR->addObject(tri1);
    stR->addObject(tri2);
    stR->addObject(rec1);
    stR->setLabel("spaceship");
    return stR;
}

GStack* createSpaceShip(GLfloat x, GLfloat y, Vec2 scale) {
    GShape *t1 = createTriangle(175.0f, 280.0f, 150.0f, 100.f);
    t1->rgb(0.174509f, 0.3098f, 0.7058f);
    t1->setLabel("spaceship top_Triangle");
    
    GShape *r1 = createRectangle(175.0f, 30.0f, 150.f, 250.f);
    r1->rgb(0.174509f, 0.3098f, 0.7058f);
    r1->setLabel("spaceship item");
    
    GShape *t2 = createTriangle(0.00f, 30.0f, 500.0f, 250.0f);
    t2->rgb(0.174509f, 0.3098f, 0.7058f);
    t2->setLabel("spaceship item");
    
    GShape *r3 = createRectangle(50.0, 0.0f, 25.0f, 130.0f);
    r3->rgb(0.174509f, 0.3098f, 0.7058f);
    r3->setLabel("spaceship item");
    
    GShape *r4 = createRectangle(430.0, 0.0f, 25.0f, 130.0f);
    r4->rgb(0.174509f, 0.3098f, 0.7058f);
    r4->setLabel("spaceship item");
    
    GShape *t3 = createTriangle(43.0f, -0.00f, 40.f, 20.f, true);
    t3->rgb(0.774509f, 0.1098f, 0.3058f);
    t3->setLabel("spaceship item");
    
    GShape *t4 = createTriangle(423, -0.00f, 40.f, 20.f, true);
    t4->rgb(0.774509f, 0.1098f, 0.3058f);
    t4->setLabel("spaceship item");
    
    GStack *st1 = new GStack(145, 20, false);
    st1->setLabel("treco de tras da nave");
    GShape *t5 = createRectTriangle(0.0, 0.00f, 23.f, -20.f);
    t5->rgb(0.974509f, 0.9098f, 0.8058f);
    st1->addObject(t5);
    GShape *t6 = createRectTriangle(200.f, 0.00f, -23.f, -20);
    t6->rgb(0.974509f, 0.9098f, 0.8058f);
    st1->addObject(t6);
    GShape *r5 = createRectangle(23, 0.00f, 155, -20.f);
    r5->rgb(0.974509f, 0.9098f, 0.8058f);
    st1->addObject(r5);
    st1->setLabel("spaceship stack item");
    
    GStack *st = new GStack(0, 0, true);
    st->addObject(r1);
    st->addObject(t1);
    st->addObject(t2);
    st->addObject(r3);
    st->addObject(r4);
    st->addObject(t3);
    st->addObject(t4);
    st->addObject(st1);
    st->setScale(scale);
    st->setItemSpawnPos(t1->getItemSpawnPos().x * scale.x, t1->getItemSpawnPos().y * scale.y);
    
    
    GStack *stR = new GStack(x, y+5);
    stR->setItemSpawnPos(st->getItemSpawnPos().x, st->getItemSpawnPos().y);
    stR->addObject(st);
    stR->boxSize.x = 500.0 * scale.x;
    
    stR->setLabel("spaceship");
    return stR;
}
