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
        objects[i]->setpropagatedScale(scale);
    }
    this->recomputeTotalScale();
}

void GStack::setpropagatedScale(Vec2 propagatedScale) {
    this->propagatedScale = propagatedScale;
    this->recomputeTotalScale();
    // propragate this element's scale combined with the parent's scale (the propagated one)
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->setpropagatedScale(totalScale);
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

GObject* GStack::testColision(std::vector<GObject*> &objects, string label) {
    throw "Not implemented";
    return NULL;
}

std::vector<std::pair<Vec2, Vec2>> GStack::getSubLines() {
    throw "Not implemented";
    return vector<pair<Vec2, Vec2>>();
}


void GStack::destroy() {
    for(int i = 0; i < objects.size(); i++) {
        free(objects[i]);
    }
}

GStack::~GStack() {
    destroy();
}
