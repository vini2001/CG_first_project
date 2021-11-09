#include "GStack.hpp"
#include "GObject.hpp"
#include <iostream>

GStack::GStack(GLfloat x, GLfloat y) : GObject(x, y) {
    this->parent = false;
}

GStack::GStack(GLfloat x, GLfloat y, bool parent) : GObject(x, y), parent(parent) {}

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

void GStack::prepare() {
    this->prepare(0.0f, 0.0f);
}

void GStack::prepare(GLfloat addX, GLfloat addY) {
    GObject::prepare();
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->prepare(parent ? (x+addX) : (x * scale.x * propagatedScale.x + addX), parent ? (y+addY) : (y * scale.y * propagatedScale.y + addY));
    }
}

void GStack::draw(GLenum mode, GLsizei count, GLenum type, const void* indices) {
    if(toUpdate) prepare();
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->draw(mode, count, type, indices);
    }
}

void GStack::bind() {
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->bind();
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
        free(objects[i]);
    }
}

GStack::~GStack() {
    destroy();
}
