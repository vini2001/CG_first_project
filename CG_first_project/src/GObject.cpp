#include <stdio.h>
#include "GObject.hpp"
#include <iostream>

using namespace std;

void GObject::draw(GLenum mode, GLsizei count, GLenum type, const void* indices) {
    if(toUpdate) prepare();
    this->bind();
    glDrawElements(mode, count, type, indices);
}

Vec2 GObject::getScale() {
    return scale;
}

void GObject::recomputeTotalScale(){
    this->totalScale.x = this->scale.x * this->propagatedScale.x;
    this->totalScale.y = this->scale.y * this->propagatedScale.y;
}

void GObject::prepare() {
    toUpdate = false;
}

void GObject::setParent(GObject *p) {
    parent = p;
}

void GObject::addPos(Vec2 change) {
    x += change.x;
    y += change.y;
    itemSpawnPos.x += change.x;
    itemSpawnPos.y += change.y;
    toUpdate = true;
}

bool GObject::update() {
    x += speed.x;
    y += speed.y;
    
    itemSpawnPos.x += speed.x;
    itemSpawnPos.y += speed.y;
    
    if(speed.x != 0.0 || speed.y != 0.0)
        toUpdate = true;
    
    return false;
}

void GObject::setSpeed(GLfloat x, GLfloat y) {
    this->speed.x = x;
    this->speed.y = y;
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
