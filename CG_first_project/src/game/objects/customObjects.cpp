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
    obj->addVertice(&v1);
    obj->addVertice(&v2);
    obj->addVertice(&v3);
    obj->addVertice(&v4);
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
    obj->addVertice(&v1);
    obj->addVertice(&v2);
    obj->addVertice(&v3);
    obj->addVertice(&v4);
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
    obj->addVertice(&v1);
    obj->addVertice(&v2);
    obj->addVertice(&v3);
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
    obj->addVertice(&v1);
    obj->addVertice(&v2);
    obj->addVertice(&v3);
    obj->addTriangle(v1, v2, v3);
    return obj;
}

GStack* createBullet(GLfloat x, GLfloat y) {
    GStack *bulletS = new GStack(x, y);
    GShape *bullet = createRectangle(0, 0, 7, 20);
    bulletS->addObject(bullet);
    bullet->setLabel("bullet-part-1");
    GShape *bullet2 = createRectangle(0, 20, 7, 7);
    bullet2->rgb(0.8, 0.1, 0.3);
    bulletS->addObject(bullet2);
    bullet2->setLabel("bullet-part-2");
    GShape *bullet3 = createTriangle(0, 27, 7, 3);
    bullet3->rgb(0.8, 0.1, 0.3);
    bulletS->addObject(bullet3);
    bullet3->setLabel("bullet-part-3");
    GShape *bullet4 = createTriangle(1.5, 0, 4, -100);
    bullet4->rgb(0.8, 0.8, 0.1);
    bulletS->addObject(bullet4);
    bullet4->setLabel("bullet-part-1");
    bullet4->vertices[2].cr = 0;
    bullet4->vertices[2].cg = 0;
    bullet4->vertices[2].cb = 0;
    
    GShape *colisionBox = createRectangle(0, -20, 7, 50);
    bulletS->colisionBox = colisionBox;
    
    return bulletS;
}

// draw Square with indices and vertices on an object
void drawSquare(GShape *obj, GVertice v1, GVertice v2, GVertice v3, GVertice v4) {
    obj->addVertice(&v1); obj->addVertice(&v2); obj->addVertice(&v3); obj->addVertice(&v4);
    obj->addTriangle(v1, v2, v3);
    obj->addTriangle(v2, v3, v4);
}

// draw Triangle with indices and vertices on an object
void drawTriangle(GShape *obj, GVertice v1, GVertice v2, GVertice v3) {
    obj->addVertice(&v1); obj->addVertice(&v2); obj->addVertice(&v3);
    obj->addTriangle(v1, v2, v3);
}

GStack* createSpaceShip(GLfloat x, GLfloat y, Vec2 scale) {
    Vec3 blueD(0.074509f, 0.2098f, 0.59058f);
    Vec3 blueDD(0.074509f, 0.1098f, 0.39058f);
    Vec3 blue(0.174509f, 0.3098f, 0.7058f);
    
    GShape *t1 = new GShape(175.0f, 280.0f);
    drawTriangle(t1, GVertice(0, 0, blue), GVertice(150, 0, blue), GVertice(75, 100, blueDD));
    t1->setLabel("spaceship top_Triangle");
    t1->setItemSpawnPos(75, 100);
    
    GShape *r1 = createRectangle(175.0f, 30.0f, 150.f, 250.f);
    r1->rgb(0.174509f, 0.3098f, 0.7058f);
    r1->setLabel("spaceship item");
    
    GShape *t2 = new GShape(0.00f, 30.0f);
    drawTriangle(t2, GVertice(0, 0, blueD), GVertice(500, 0, blueD), GVertice(250, 250, blue));
    
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
    
    GStack *st1 = new GStack(145, 24, false);
    st1->setLabel("treco de tras da nave");
    GShape *t5 = createRectTriangle(0.0, 0.00f, 23.f, -20.f);
    t5->rgb(0.074509f, 0.1098f, 0.39058f);
    st1->addObject(t5);
    GShape *t6 = createRectTriangle(200.f, 0.00f, -23.f, -20);
    t6->rgb(0.074509f, 0.1098f, 0.39058f);
    st1->addObject(t6);
    GShape *r5 = createRectangle(23, 0.00f, 155, -20.f);
    r5->rgb(0.074509f, 0.1098f, 0.39058f);
    st1->addObject(r5);
    st1->setLabel("spaceship stack item");

    GShape *backObjs = new GShape(55, 10);
    Vec3 cr(0.074509f, 0.2098f, 0.59058f);
    Vec3 white(1,1,1);
    float bwid = 8;
    float bheight = 40;
    float space = 4.81;
    for(int i = 1; i < 10; i++) {
        drawSquare(backObjs, GVertice(0 + i*bwid*space, 0, cr), GVertice(bwid + i*bwid*space, 0, cr), GVertice(0 + i*bwid*space, bheight, white), GVertice(bwid + i*bwid*space, bheight, white));
    }
    
    GStack *st = new GStack(0, 0, true);
    st->addObject(r1);
    st->addObject(t1);
    st->addObject(t2);
    st->addObject(r3);
    st->addObject(r4);
    st->addObject(t3);
    st->addObject(t4);
    st->addObject(backObjs);
    st->addObject(st1);
    st->setScale(scale);
    st->setItemSpawnPos(t1->getItemSpawnPos().x * scale.x, t1->getItemSpawnPos().y * scale.y);
    
    
    GStack *stR = new GStack(x, y+5);
    stR->setItemSpawnPos(st->getItemSpawnPos().x, st->getItemSpawnPos().y);
    stR->addObject(st);
    stR->boxSize.x = 500.0 * scale.x;
    
    GShape *colisionBox = createTriangle(-20*scale.x, 0, 540*scale.x, 378*scale.y);
    stR->colisionBox = colisionBox;
    
    stR->setLabel("spaceship");
    return stR;
}
