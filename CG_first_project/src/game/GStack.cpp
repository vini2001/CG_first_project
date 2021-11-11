#include "GStack.hpp"
#include "GObject.hpp"
#include <iostream>

GStack::GStack(GLfloat x, GLfloat y) : GObject(x, y) {
    this->parent = false;
    classType = "stack";
}

GStack::GStack(GLfloat x, GLfloat y, bool parent) : GObject(x, y), parent(parent) {
    classType = "stack";
}

void GStack::addObject(GObject *obj) {
    obj->setParent(this);
    objects.push_back(obj);
}

void GStack::setScale(Vec2 scale) {
    this->scale = scale;
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->setPropagatedScale(scale);
    }
    this->recomputeTotalScale();
}

void GStack::setPropagatedScale(Vec2 propagatedScale) {
    this->propagatedScale = propagatedScale;
    this->recomputeTotalScale();
    // propragate this element's scale combined with the parent's scale (the propagated one)
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->setPropagatedScale(totalScale);
    }
}

void GStack::prepare(GLfloat* vArray, long &arrayPos, GLuint *indices, long &indicesPos) {
    this->prepare(vArray, arrayPos, indices, indicesPos, 0.0f, 0.0f);
}

void GStack::prepare(GLfloat *vArray, long &arrayPos, GLuint *indices, long &indicesPos, GLfloat addX, GLfloat addY) {
    GObject::prepare(vArray, arrayPos, indices, indicesPos);
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->prepare(vArray, arrayPos, indices, indicesPos, parent ? (x+addX) : (x * scale.x * propagatedScale.x + addX), parent ? (y+addY) : (y * scale.y * propagatedScale.y + addY));
    }
}

void GStack::getSizes(int &vertices, int &indices, int &triangles) {
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->getSizes(vertices, indices, triangles);
    }
}

std::vector<std::pair<Vec2, Vec2>> GStack::getSubLines() {
    return this->getSubLines(0.0f, 0.0f);
}

std::vector<std::pair<Vec2, Vec2>> GStack::getSubLines(GLfloat addX, GLfloat addY) {
    
    vector<pair<Vec2, Vec2>> res;
    
    for(int i = 0; i < objects.size(); i++) {
        vector<pair<Vec2, Vec2>> someSubs = objects[i]->getSubLines(parent ? (x+addX) : (x * scale.x * propagatedScale.x + addX), parent ? (y+addY) : (y * scale.y * propagatedScale.y + addY));
        for(int j = 0; j < someSubs.size(); j++) res.push_back(someSubs[j]);
    }
    
    return res;
}


void GStack::destroy() {
    for(int i = 0; i < objects.size(); i++) {
        if(objects[i]->classType == "stack") {
            delete dynamic_cast<GStack*>(objects[i]);
        }else if(objects[i]->classType == "shape") {
            delete dynamic_cast<GShape*>(objects[i]);
        }
    }
}

GStack::~GStack() {
    destroy();
}
