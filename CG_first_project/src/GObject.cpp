#include <stdio.h>
#include "GObject.hpp"
#include <iostream>

void GObject::draw(GLenum mode, GLsizei count, GLenum type, const void* indices) {
    this->bind();
    glDrawElements(mode, count, type, indices);
}

GLfloat GObject::getScale() {
    return scale;
}


void GObject::addPos(vec2 change) {
    x += change[0];
}


GObject::GObject() {}
GObject::GObject(GLfloat x, GLfloat y) : x(x), y(y) {}
