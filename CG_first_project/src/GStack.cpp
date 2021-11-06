#include "GStack.hpp"
#include "GObject.hpp"
#include <iostream>

GStack::GStack(GLfloat x, GLfloat y) : GObject(x, y) {
    this->parent = false;
}

GStack::GStack(GLfloat x, GLfloat y, bool parent) : GObject(x, y), parent(parent) {}

void GStack::addObject(GObject *obj) {
    objects.push_back(obj);
}

void GStack::setScale(GLfloat scale) {
    this->scale = scale;
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->setScale(scale);
    }
}

void GStack::prepare() {
    this->prepare(0.0f, 0.0f);
}

void GStack::prepare(GLfloat addX, GLfloat addY) {
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->prepare(parent ? (x + addX) : (x*scale + addX), parent ? (y+addY) : (y*scale+addY));
    }
}

void GStack::draw(GLenum mode, GLsizei count, GLenum type, const void* indices) {
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->draw(mode, count, type, indices);
    }
}

void GStack::bind() {
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->bind();
    }
}

GStack::~GStack() {
    for(int i = 0; i < objects.size(); i++) {
        free(objects[i]);
    }
}
