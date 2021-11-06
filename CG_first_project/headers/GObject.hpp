#ifndef GObject_hpp
#define GObject_hpp
#include <glad/glad.h>
#include "linmath.h"
#include <queue>

class GObject {
   public:
        GObject(GLfloat x, GLfloat y);
        GObject();
        virtual void prepare();
        virtual void prepare(GLfloat addX, GLfloat addY) = 0;
        virtual void bind() = 0;
        virtual void setScale(GLfloat scale) = 0;
        bool update();
        GLfloat getScale();
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
        GLfloat x, y;
        GLfloat scale = 1;
        vec2 speed;
        vec2 itemSpawnPos;
        bool toUpdate = false;
};

#endif /* GObject_hpp */
