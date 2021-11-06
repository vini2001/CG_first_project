#include "GShape.hpp"

#include <iostream>

GTriangle::GTriangle(GVertice aa, GVertice bb, GVertice cc) : a(aa), b(bb), c(cc) {}

GVertice::GVertice(GLfloat x, GLfloat y) : x(x), y(y) {
    this->cr = 0.0f;
    this->cg = 0.0f;
    this->cb = 0.0f;
}

GVertice::GVertice(GLfloat x, GLfloat y, GLfloat cr, GLfloat cg, GLfloat cb) : x(x), y(y) {
    this->cr = cr;
    this->cg = cg;
    this->cb = cb;
}

GShape::GShape(GLfloat x, GLfloat y): GObject(x, y){
    this->vArray = NULL;
    this->vao = NULL;
    this->vbo1 = NULL;
    this->ebo1 = NULL;
}

GShape::GShape(){}

void GShape::rgb(GLfloat r, GLfloat g, GLfloat b){
    for(int i = 0; i < vertices.size(); i++) {
        vertices[i].cr = r;
        vertices[i].cg = g;
        vertices[i].cb = b;
    }
}

GLfloat* GShape::getVerticesArray() {
    if(this->vArray != NULL) delete this->vArray;
    
    // 6 => 3 coords & 3 colors
    this->vArray = (GLfloat*) malloc(sizeof(GLfloat) * 6 * vertices.size());
    
    for(int i = 0; i < vertices.size(); i++) {
        this->vArray[6*i+0] = vertices[i].x + x;
        this->vArray[6*i+1] = vertices[i].y + y;
        this->vArray[6*i+2] = 0.0f;
        
        this->vArray[6*i+3] = vertices[i].cr;
        this->vArray[6*i+4] = vertices[i].cg;
        this->vArray[6*i+5] = vertices[i].cb;
    }
    
    return this->vArray;
}

int GShape::getSizeVertices() {
    // 6 => 3 coords & 3 colors
    return (int)(sizeof(GLfloat) * 6 * vertices.size());
}

void GShape::addVertice(GVertice &v) {
    vertices.push_back(v);
    v.id = verticesIdsCount++;
}

void GShape::addTriangle(GVertice v1, GVertice v2, GVertice v3) {
    GTriangle tri(v1, v2, v3);
    triangles.push_back(tri);
}

void GShape::setScale(GLfloat scale) {
    this->scale = scale;
}

void GShape::prepare() {
    this->prepare(0.0f, 0.0f);
}

void GShape::prepare(GLfloat addX, GLfloat addY) {
    GObject::prepare();
    if(vao != NULL) delete vao;
    
    vao = new VAO();
    vao->bind();
    getVerticesArray();
    
    for(int i = 0; i < vertices.size(); i++) {
        vArray[6*i + 0] = (vArray[6*i + 0]) * scale  + addX + speed[0];
        vArray[6*i + 1] = (vArray[6*i + 1]) * scale  + addY + speed[1];
    }
    
    vbo1 = new VBO(vArray, getSizeVertices());
    
    int sizeIndices = (int) (sizeof(GLuint) * triangles.size() * 3);
    indices = (GLuint*) malloc(sizeIndices);
    for(int i = 0; i < triangles.size(); i++) {
        indices[3*i + 0] = triangles[i].a.id;
        indices[3*i + 1] = triangles[i].b.id;
        indices[3*i + 2] = triangles[i].c.id;
    }
    
    ebo1 = new EBO(indices, sizeIndices);
    
    vao->linkAttrib(*vbo1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    vao->linkAttrib(*vbo1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    
    vao->unbind();
    delete vbo1;
    delete ebo1;
}

void GShape::bind() {
    vao->bind();
}

void GShape::destroy() {
    cout << "GShape destroy" << endl;
    free(vArray);
    free(indices);
    
    if(vao != NULL) {
        delete vao;
    }
}

GShape::~GShape() {
    destroy();
}
