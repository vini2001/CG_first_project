#include <stdio.h>
#include "GObject.hpp"
#include <iostream>
#include "globals.hpp"

using namespace std;

Vec2 GObject::getScale() {
    return scale;
}

void GObject::recomputeTotalScale(){
    this->totalScale.x = this->scale.x * this->propagatedScale.x;
    this->totalScale.y = this->scale.y * this->propagatedScale.y;
}

void GObject::prepare(GLfloat* vArray, long &arrayPos, GLuint *indices, long &indicesPos) {}

void GObject::setParent(GObject *p) {
    parent = p;
}

void GObject::addSpeed(Vec2 change) {
    speed.x += change.x;
    speed.y += change.y;
}

bool GObject::update() {
    
    Vec2 speed = this->speed;
    speed.x = (speed.x/10)*framesSinceRender;
    speed.y = (speed.y/10)*framesSinceRender;
    
    x += speed.x;
    y += speed.y;
    
    itemSpawnPos.x += speed.x;
    itemSpawnPos.y += speed.y;

    
    return false;
}

void GObject::setSpeed(Vec2 v) {
    this->speed = v;
}

Vec2 GObject::getSpeed() {
    return this->speed;
}

void GObject::setItemSpawnPos(GLfloat x, GLfloat y) {
    this->itemSpawnPos.x = x + this->x;
    this->itemSpawnPos.y = y + this->y;
}

Vec2 GObject::getItemSpawnPos() {
    return itemSpawnPos;
}

bool GObject::shouldDestroy(long currentTime) {
    return destroyAt != -1 && currentTime > destroyAt;
}

void GObject::setLabel(std::string label) {
    this->label = label;
}

std::string GObject::getLabel() {
    return label;
}

void GObject::init() {
    boxSize = Vec2(0, 0);
    scale = Vec2(1,1);
    propagatedScale = Vec2(1,1);
    totalScale = Vec2(1,1);
    speed = Vec2(0,0);
    itemSpawnPos = Vec2(0,0);
}

float ccw(Vec2 A, Vec2 B, Vec2 C) {
    return (C.y-A.y) * (B.x-A.x) > (B.y-A.y) * (C.x-A.x);
}

// Return true if line segments AB and CD intersect
bool intersect(Vec2 A, Vec2 B, Vec2 C, Vec2 D){
    return ccw(A,C,D) != ccw(B,C,D) and ccw(A,B,C) != ccw(A,B,D);
}

GObject* GObject::testColision(std::vector<GObject*> &objects, string label) {
    for(int i = 0; i < objects.size(); i++) {
        GObject *o = objects[i];
        
        if(o->getLabel() == label) {
            vector<pair<Vec2, Vec2>> subLinesOther = o->getSubLines();
            vector<pair<Vec2, Vec2>> subLineThis = getSubLines();
            
            for(int t = 0; t < subLineThis.size(); t++) {
                for(int j = 0; j < subLinesOther.size(); j++) {
//                    cout << "Test " << "((" << subLineThis[t].first.x << "," << subLineThis[t].first.y << "), (" <<  subLineThis[t].second.x << "," << subLineThis[t].second.y << "))" << ", " << "((" << subLinesOther[j].first.x << "," << subLinesOther[j].first.y << "), (" <<  subLinesOther[j].second.x << "," << subLinesOther[j].second.y << "))" << endl;
                    if(intersect(subLineThis[t].first, subLineThis[t].second, subLinesOther[j].first, subLinesOther[j].second)) {
                        return o;
                    }
                }
            }
//            std::cout << "testing colision with bullet" << std::endl;
        }
    }
    return NULL;
}

GObject::GObject() {
    init();
}
GObject::GObject(GLfloat x, GLfloat y) : x(x), y(y) {
    init();
}
