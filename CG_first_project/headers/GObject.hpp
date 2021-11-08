#ifndef GObject_hpp
#define GObject_hpp
#include <glad/glad.h>
#include "linmath.h"
#include <queue>
#include <string>
#include "vectors.hpp"


class GObject {
   public:
        GLfloat x, y;
        Vec2 boxSize;
        GObject(GLfloat x, GLfloat y);
        GObject();
        virtual void prepare();
        virtual void prepare(GLfloat addX, GLfloat addY) = 0;
        virtual void bind() = 0;
        virtual void setScale(Vec2 scale) = 0;
        virtual void setPropagatedScale(Vec2 scale) = 0;
        bool update();
        Vec2 getScale();
        void addPos(Vec2 change);
        void setSpeed(GLfloat x, GLfloat y);
        void setItemSpawnPos(GLfloat x, GLfloat y);
        Vec2 getItemSpawnPos();
        bool shouldDestroy(long currentTime);
        std::string getLabel();
        void setLabel(std::string label);
        void setParent(GObject *p);
    
        virtual std::vector<std::pair<Vec2, Vec2>> getSubLines() = 0;
    
        virtual GObject* testColision(std::vector<GObject*> &objects, std::string label) = 0;
    
        virtual void destroy() = 0;
        
        long destroyAt = -1;
    
        /// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawElements.xhtml
        virtual void draw(GLenum mode, GLsizei count, GLenum type, const void* indices);
    
    protected:
        // the individual element scale
        Vec2 scale;
    
        // scale that comes from the parent objects
        Vec2 propagatedScale;
    
        // scale and propagated scales combined
        Vec2 totalScale;
    
        Vec2 speed;
        Vec2 itemSpawnPos;
        bool toUpdate = false;
        std::string label;
        GObject *parent = NULL;
        
        void recomputeTotalScale();
    
    private:
        void init();
};

#endif /* GObject_hpp */
