#ifndef GStack_hpp
#define GStack_hpp

#include <stdio.h>
#include "GShape.hpp"
#include "GObject.hpp"
#include <vector>
#include "vectors.hpp"

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
    void setScale(Vec2 scale);
    void setPropagatedScale(Vec2 scale);
    std::vector<std::pair<Vec2, Vec2>> getSubLines();
    GObject* testColision(std::vector<GObject*> &objects, string label);
    
    void destroy();
    
    ~GStack();
};


#endif /* GStack_hpp */
