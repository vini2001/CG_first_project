#ifndef GStack_hpp
#define GStack_hpp

#include <stdio.h>
#include "GShape.hpp"
#include "GObject.hpp"
#include <vector>

class GStack : public GObject {
private:
    vector<GObject*> objects;
    bool parent;
public:
    GStack(GLfloat x, GLfloat y);
    GStack(GLfloat x, GLfloat y, bool parent);
    void addObject(GObject*);
    void prepare();
    void prepare(GLfloat addX, GLfloat addY);
    void bind();
    void draw(GLenum mode, GLsizei count, GLenum type, const void* indices);
    void setScale(GLfloat scale);
    void destroy();
    
    ~GStack();
};


#endif /* GStack_hpp */
