#ifndef GObject_hpp
#define GObject_hpp
#include <glad/glad.h>
#include "linmath.h"
#include <queue>

class GObject {
   public:
        GLfloat x, y;
        vec2 boxSize = {0.0, 0.0};
        GObject(GLfloat x, GLfloat y);
        GObject();
        virtual void prepare();
        virtual void prepare(GLfloat addX, GLfloat addY) = 0;
        virtual void bind() = 0;
        virtual void setScale(vec2 scale) = 0;
        bool update();
        float* getScale();
        void addPos(vec2 change);
        void setSpeed(GLfloat x, GLfloat y);
        void setItemSpawnPos(GLfloat x, GLfloat y);
        float* getItemSpawnPos();
        bool shouldDestroy(long currentTime);
    
        virtual void destroy() = 0;
        
        long destroyAt = -1;
    
        /// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawElements.xhtml
        virtual void draw(GLenum mode, GLsizei count, GLenum type, const void* indices);
    
    protected:
        vec2 scale = {1.0, 1.0};
        vec2 speed;
        vec2 itemSpawnPos;
        bool toUpdate = false;
};

#endif /* GObject_hpp */
