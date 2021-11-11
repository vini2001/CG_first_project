#ifndef GObject_hpp
#define GObject_hpp
#include <glad/glad.h>
#include "linmath.h"
#include <queue>
#include <string>
#include "vectors.hpp"
#include <string>


class GObject {
   public:
        GLfloat x, y;
        Vec2 boxSize;
        std::string classType;
        GObject *colisionBox = NULL;
    
        GObject(GLfloat x, GLfloat y);
        GObject();
        virtual void prepare(GLfloat* vArray, long &arrayPos, GLuint *indices, long &indicesPos);
        virtual void prepare(GLfloat* vArray, long &arrayPos, GLuint *indices, long &indicesPos, GLfloat addX, GLfloat addY) = 0;
        virtual void setScale(Vec2 scale) = 0;
        virtual void setPropagatedScale(Vec2 scale) = 0;
        bool update();
        Vec2 getScale();
        void addSpeed(Vec2 change);
        void setSpeed(Vec2 change);
        Vec2 getSpeed();
        void setItemSpawnPos(GLfloat x, GLfloat y);
        Vec2 getItemSpawnPos();
        bool shouldDestroy(long currentTime);
        std::string getLabel();
        void setLabel(std::string label);
        void setParent(GObject *p);
    
        virtual std::vector<std::pair<Vec2, Vec2>> getSubLines() = 0;
        virtual std::vector<std::pair<Vec2, Vec2>> getSubLines(GLfloat addX, GLfloat addY) = 0;
        virtual void getSizes(int &vertices, int &indices, int &triangles) = 0;
    
    GObject* testColision(std::vector<GObject*> &objects, std::string label);
    
        virtual void destroy();
        
        long destroyAt = -1;
    
    protected:
        // the individual element scale
        Vec2 scale;
    
        // scale that comes from the parent objects
        Vec2 propagatedScale;
    
        // scale and propagated scales combined
        Vec2 totalScale;
    
        Vec2 speed;
        Vec2 itemSpawnPos;

        std::string label;
        GObject *parent = NULL;
        
        void recomputeTotalScale();
    
    private:
        void init();
};

#endif /* GObject_hpp */
