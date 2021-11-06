#include <stdio.h>
#include "GShape.hpp"
#include "customObjects.h"


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

GStack* createSpaceShip(GLfloat x, GLfloat y, GLfloat scale) {
    GShape *t1 = createTriangle(0.175f, 0.28f, 0.15f, 0.1f);
    t1->rgb(0.174509f, 0.3098f, 0.7058f);
    
    GShape *r1 = createRectangle(0.175f, 0.03f, 0.15f, 0.25f);
    r1->rgb(0.214509f, 0.25098f, 0.7058f);
    
    GShape *t2 = createTriangle(0.00f, 0.03f, 0.5f, 0.25f);
    t2->rgb(0.174509f, 0.3098f, 0.7058f);
    
    GShape *r3 = createRectangle(0.05, 0.0f, 0.025f, 0.13f);
    r3->rgb(0.174509f, 0.3098f, 0.7058f);
    
    GShape *r4 = createRectangle(0.43, 0.0f, 0.025f, 0.13f);
    r4->rgb(0.174509f, 0.3098f, 0.7058f);
    
    GShape *t3 = createTriangle(0.043, -0.00f, 0.04f, 0.02f, true);
    t3->rgb(0.774509f, 0.1098f, 0.3058f);
    
    GShape *t4 = createTriangle(0.423, -0.00f, 0.04f, 0.02f, true);
    t4->rgb(0.774509f, 0.1098f, 0.3058f);
    
    GStack *st1 = new GStack(0.145, 0.02f);
    GShape *t5 = createRectTriangle(0.0, 0.00f, 0.023f, -0.02f);
    t5->rgb(0.974509f, 0.9098f, 0.8058f);
    st1->addObject(t5);
    GShape *t6 = createRectTriangle(0.20, 0.00f, -0.023f, -0.02f);
    t6->rgb(0.974509f, 0.9098f, 0.8058f);
    st1->addObject(t6);
    GShape *r5 = createRectangle(0.023, 0.00f, 0.155f, -0.02f);
    r5->rgb(0.974509f, 0.9098f, 0.8058f);
    st1->addObject(r5);
    
    GStack *st = new GStack(x, y, true);
    st->addObject(r1);
    st->addObject(t1);
    st->addObject(t2);
    st->addObject(r3);
    st->addObject(r4);
    st->addObject(t3);
    st->addObject(t4);
    st->addObject(st1);
    st->setScale(scale);
    
    return st;
}
