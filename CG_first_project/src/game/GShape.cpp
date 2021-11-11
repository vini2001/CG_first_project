#include "GShape.hpp"

#include <iostream>
#include "globals.hpp"

GTriangle::GTriangle(GVertice aa, GVertice bb, GVertice cc) : a(aa), b(bb), c(cc) {}

GVertice::GVertice(GLfloat x, GLfloat y) : x(x), y(y) {
    this->cr = 0.0f;
    this->cg = 0.0f;
    this->cb = 0.0f;
}

GVertice::GVertice(GLfloat x, GLfloat y, Vec3 rgb) : x(x), y(y) {
    this->cr = rgb.r;
    this->cg = rgb.g;
    this->cb = rgb.b;
}

GVertice::GVertice(GLfloat x, GLfloat y, GLfloat cr, GLfloat cg, GLfloat cb) : x(x), y(y) {
    this->cr = cr;
    this->cg = cg;
    this->cb = cb;
}

Vec2 GVertice::toVec() {
    return Vec2(x, y);
}

GShape::GShape(GLfloat x, GLfloat y): GObject(x, y){}

GShape::GShape(){}


void GShape::rgb(GLfloat r, GLfloat g, GLfloat b){
    for(int i = 0; i < vertices.size(); i++) {
        vertices[i].cr = r;
        vertices[i].cg = g;
        vertices[i].cb = b;
    }
}

GLfloat* GShape::getVerticesArray() {
    
    for(int i = 0; i < vertices.size(); i++) {
//        this->vArray[6*i+0] = vertices[i].x * totalScale.x + x * propagatedScale.x;
//        this->vArray[6*i+1] = vertices[i].y * totalScale.y + y * propagatedScale.y;
//        this->vArray[6*i+2] = 0.0f;
//
//        this->vArray[6*i+3] = vertices[i].cr;
//        this->vArray[6*i+4] = vertices[i].cg;
//        this->vArray[6*i+5] = vertices[i].cb;
    }
    
//    return this->vArray;
    return NULL;
}

void GShape::getSizes(int &vertices, int &indices, int &triangles) {
    // 6 => 3 coords & 3 colors
    vertices += (sizeof(GLfloat) * 6 * this->vertices.size());
    indices += (int)(sizeof(GLuint) * this->triangles.size() * 3);
    triangles += this->triangles.size();
}


void GShape::addVertice(GVertice &v) {
    vertices.push_back(v);
    v.id = verticesIdsCount++;
}

void GShape::addTriangle(GVertice v1, GVertice v2, GVertice v3) {
    GTriangle tri(v1, v2, v3);
    triangles.push_back(tri);
}

void GShape::setScale(Vec2 scale) {
    this->scale = scale;
    this->recomputeTotalScale();
}

void GShape::setPropagatedScale(Vec2 propagatedScale) {
    this->propagatedScale = propagatedScale;
    this->recomputeTotalScale();
}

void GShape::prepare(GLfloat* vArray, long &arrayPos, GLuint *indices, long &indicesPos) {
    this->prepare(vArray, arrayPos, indices, indicesPos, 0.0f, 0.0f);
}

GLfloat normalizeCoordX(GLfloat coordU) {
    return coordU / (game::width/2);
}

GLfloat normalizeCoordY(GLfloat coordU) {
    return coordU / (game::height/2);
}

void GShape::prepare(GLfloat* vArray, long &arrayPos, GLuint *indices, long &indicesPos, GLfloat addX, GLfloat addY) {
    GObject::prepare(vArray, arrayPos, indices, indicesPos);
    
    // Each vertice represents 6 positions on the vArray array. So for each 6 positions, 1 vertice has been added. At this point, we need to know how many vertices have been added to determine the starting ID of this shape's vertices
    int startingId = (int) arrayPos/6;
    
    for(int i = 0; i < vertices.size(); i++) {
        vArray[arrayPos++] = normalizeCoordX(vertices[i].x * totalScale.x + x * propagatedScale.x + addX);
        vArray[arrayPos++] = normalizeCoordY(vertices[i].y * totalScale.y + y * propagatedScale.y + addY);
        vArray[arrayPos++] = 0.0f;

        vArray[arrayPos++] = vertices[i].cr;
        vArray[arrayPos++] = vertices[i].cg;
        vArray[arrayPos++] = vertices[i].cb;
    }

    
//    int sizeIndices = (int) (sizeof(GLuint) * triangles.size() * 3);
//    if(indices == NULL) {
//        indices = (GLuint*) malloc(sizeIndices);
        for(int i = 0; i < triangles.size(); i++) {
            indices[indicesPos++] = triangles[i].a.id + startingId;
            indices[indicesPos++] = triangles[i].b.id + startingId;
            indices[indicesPos++] = triangles[i].c.id + startingId;
        }
//    }
//    ebo1 = new EBO(indices, sizeIndices);
    
    
//    vao->linkAttrib(*vbo1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
//    vao->linkAttrib(*vbo1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    
//    vao->unbind();
//    delete vbo1;
//    delete ebo1;
}

void GShape::bind() {
//    vao->bind();
}

void GShape::destroy() {
//    free(vArray);
//    free(indices);
//    
//    if(vao != NULL) {
//        delete vao;
//    }
}

std::vector<std::pair<Vec2, Vec2>> GShape::getSubLines() {
    return this->getSubLines(0.0f, 0.0f);
}

std::vector<std::pair<Vec2, Vec2>> GShape::getSubLines(GLfloat addX, GLfloat addY) {
    vector<pair<Vec2, Vec2>> res;
    
    // get every line segment that forms the shape
    for(int t = 0; t < triangles.size(); t++) {
        pair<Vec2, Vec2> a = { triangles[t].a.toVec(), triangles[t].b.toVec() };
        pair<Vec2, Vec2> b = { triangles[t].b.toVec(), triangles[t].c.toVec() };
        pair<Vec2, Vec2> c = { triangles[t].c.toVec(), triangles[t].a.toVec() };
        res.push_back(a);
        res.push_back(b);
        res.push_back(c);
    }
    
    // add the position of the shape to every line segment relative position, so we get the real position of each segment
    for(int i = 0; i < res.size(); i++) {
        res[i].first.x = res[i].first.x*totalScale.x +  x*propagatedScale.x + addX;
        res[i].first.y = res[i].first.y*totalScale.y +  y*propagatedScale.y + addY;
        res[i].second.x = res[i].second.x*totalScale.x +  x*propagatedScale.x + addX;
        res[i].second.y = res[i].second.y*totalScale.y +  y*propagatedScale.y + addY;
    }
    
    return res;
}

GShape::~GShape() {
    destroy();
}
