#ifndef GObject_hpp
#define GObject_hpp
#include <glad/glad.h>
#include "linmath.h"

class GObject {
   public:
        GObject(GLfloat x, GLfloat y);
        GObject();
        virtual void prepare() = 0;
        virtual void prepare(GLfloat addX, GLfloat addY) = 0;
        virtual void bind() = 0;
        virtual void setScale(GLfloat scale) = 0;
        GLfloat getScale();
        void addPos(vec2 change);
    
        /// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawElements.xhtml
        virtual void draw(GLenum mode, GLsizei count, GLenum type, const void* indices);
    
    protected:
        GLfloat x, y;
        GLfloat scale = 1;
    
};

#endif /* GObject_hpp */
