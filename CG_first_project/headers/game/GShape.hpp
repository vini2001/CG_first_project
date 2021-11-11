#ifndef gameObject_hpp
#define gameObject_hpp

#include <stdio.h>
#include <vector>
#include "VBO.hpp"
#include "EBO.hpp"
#include "VAO.hpp"
#include "GObject.hpp"
#include "globals.hpp"

using namespace std;


class GVertice {
    public:
        GLuint id;
        GLfloat x, y;
        GLfloat cr, cg, cb;
        GVertice(GLfloat x, GLfloat y);
        GVertice(GLfloat x, GLfloat y, GLfloat cr, GLfloat cg, GLfloat cb);
        GVertice(GLfloat x, GLfloat y, Vec3 rgb);
        Vec2 toVec();
};

class GTriangle {
    public:
        GVertice a, b, c;
        GTriangle(GVertice a, GVertice b, GVertice c);
};

class GShape : public GObject {
    
    public:
        GShape(GLfloat x, GLfloat y);
        GShape();
        int ID;
        void addVertice(GVertice &v);
        void prepare(GLfloat* vArray, long &arrayPos, GLuint *indices, long &indicesPos);
        void prepare(GLfloat* vArray, long &arrayPos, GLuint *indices, long &indicesPos, GLfloat addX, GLfloat addY);
        void bind();
        void addTriangle(GVertice v1, GVertice v2, GVertice v3);
        void rgb(GLfloat r, GLfloat g, GLfloat b);
        void setScale(Vec2 scale);
        void setPropagatedScale(Vec2 scale);
        void destroy();
        std::vector<std::pair<Vec2, Vec2>> getSubLines();
        std::vector<std::pair<Vec2, Vec2>> getSubLines(GLfloat addX, GLfloat addY);
    
        vector<GVertice> vertices;
        GLfloat* getVerticesArray();
    void getSizes(int &vertices, int &indices, int &triangles);
    
        ~GShape();
    
    private:
        vector<GTriangle> triangles;
        GLuint verticesIdsCount = 0;
};

#endif
