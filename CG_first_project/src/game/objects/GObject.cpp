#include <stdio.h>
#include "GObject.hpp"
#include <iostream>
#include "globals.hpp"
#include "GShape.hpp"
#include "GStack.hpp"

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
    
    this->speed.x += acceleration.x;
    this->speed.y += acceleration.y;
    
    Vec2 speed = this->speed;
    speed.x = (speed.x/10)*framesSinceUpdate;
    speed.y = (speed.y/10)*framesSinceUpdate;
    
    x += speed.x;
    y += speed.y;
    
    itemSpawnPos.x += speed.x;
    itemSpawnPos.y += speed.y;

    
    return false;
}

void GObject::setSpeed(Vec2 v) {
    this->speed = v;
}

void GObject::setAcceleration(Vec2 v) {
    this->acceleration = v;
}

Vec2 GObject::getSpeed() {
    return this->speed;
}

Vec2 GObject::getAcceleration() {
    return this->acceleration;
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

std::string GObject::positionDebug() {
    std::string debugStr = "";
    debugStr.append("(");
    debugStr.append(to_string(x));
    debugStr.append(",");
    debugStr.append(to_string(y));
    debugStr.append(")");
    return debugStr;
}

void GObject::init() {
    boxSize = Vec2(0, 0);
    scale = Vec2(1,1);
    propagatedScale = Vec2(1,1);
    totalScale = Vec2(1,1);
    speed = Vec2(0,0);
    acceleration = Vec2(0,0);
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
        }
    }
    return NULL;
}

void GObject::destroy() {
    if(colisionBox != NULL) {
        if(colisionBox->classType == "stack") {
            delete dynamic_cast<GStack*>(colisionBox);
        }else if(colisionBox->classType == "shape") {
            delete dynamic_cast<GShape*>(colisionBox);
        }
    }
}

GObject::GObject() {
    init();
}
GObject::GObject(GLfloat x, GLfloat y) : x(x), y(y) {
    init();
}
