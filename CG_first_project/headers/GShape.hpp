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
        void prepare();
        void prepare(GLfloat addX, GLfloat addY);
        void bind();
        void addTriangle(GVertice v1, GVertice v2, GVertice v3);
        void rgb(GLfloat r, GLfloat g, GLfloat b);
        void setScale(vec2 scale);
        void destroy();
    
        vector<GVertice> vertices;
        GLfloat* getVerticesArray();
        int getSizeVertices();
    
        ~GShape();
    
    private:
        GLfloat *vArray;
        VAO* vao;
        VBO* vbo1;
        EBO* ebo1;
        GLuint* indices;
        vector<GTriangle> triangles;
        GLuint verticesIdsCount = 0;
};

#endif
