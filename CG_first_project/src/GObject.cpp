#include <stdio.h>
#include "GObject.hpp"
#include <iostream>

void GObject::draw(GLenum mode, GLsizei count, GLenum type, const void* indices) {
    if(toUpdate) prepare();
    this->bind();
    glDrawElements(mode, count, type, indices);
}

GLfloat GObject::getScale() {
    return scale;
}

void GObject::prepare() {
    toUpdate = false;
}

void GObject::addPos(vec2 change) {
    x += change[0];
    y += change[1];
    itemSpawnPos[0] += change[0];
    itemSpawnPos[1] += change[1];
    toUpdate = true;
}

bool GObject::update() {
    x += speed[0];
    y += speed[1];
    
    itemSpawnPos[0] += speed[0];
    itemSpawnPos[1] += speed[1];
    
    if(speed[0] != 0.0 || speed[1] != 0.0)
        toUpdate = true;
    
    return false;
}

void GObject::setSpeed(GLfloat x, GLfloat y) {
    this->speed[0] = x;
    this->speed[1] = y;
}

void GObject::setItemSpawnPos(GLfloat x, GLfloat y) {
    this->itemSpawnPos[0] = x + this->x;
    this->itemSpawnPos[1] = y + this->y;
}

float* GObject::getItemSpawnPos() {
    return itemSpawnPos;
}

bool GObject::shouldDestroy(long currentTime) {
    return destroyAt != -1 && currentTime > destroyAt;
}

GObject::GObject() {
    speed[0] = 0.0;
    speed[1] = 0.0;
    itemSpawnPos[0] = 0.0;
    itemSpawnPos[1] = 0.0;
}
GObject::GObject(GLfloat x, GLfloat y) : x(x), y(y) {
    speed[0] = 0.0;
    speed[1] = 0.0;
    itemSpawnPos[0] = 0.0;
    itemSpawnPos[1] = 0.0;
}
