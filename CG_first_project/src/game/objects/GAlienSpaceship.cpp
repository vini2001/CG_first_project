#include "GAlienSpaceship.hpp"
#include "customObjects.h"

GAlien::GAlien(GLfloat x, GLfloat y, int prize) : GShape(x, y + 5), prize(prize) {
    refresh();
}

void GAlien::refresh() {
    
    vertices.clear();
    triangles.clear();
    verticesIdsCount = 0;
    
    float whiteFactor = ((float)life) / (bigOne ? 200 : 100);
    Vec3 white(whiteFactor, whiteFactor, whiteFactor);
    if(prize == ALIEN_SHOOTING_PRIZE) {
        white = Vec3(0.98f, whiteFactor, whiteFactor*0.8);
    }
    drawSquare(this, GVertice(0, 0, white), GVertice(0, 50, white), GVertice(50, 0, white), GVertice(50, 50, white));
    
    Vec3 blue(0.09, 0.13, 0.43);
    Vec3 red(0.99, 0.13, 0.33);
    
    if(prize == ALIEN_SHOOTING_INTERVAL_PRIZE) {
        blue = Vec3(0.09, 0.43, 0.8);
    }else if(prize == ALIEN_SHOOTING_SPEED_PRIZE) {
        blue = Vec3(0.89, 0.43, 0.8);
    }
    
    drawTriangle(this, GVertice(-15, 35, blue), GVertice(15, 35, blue), GVertice(0, 15, blue));
    drawTriangle(this, GVertice(35, 35, blue), GVertice(65, 35, blue), GVertice(50, 15, blue));
    drawSquare(this, GVertice(-10, 50, blue), GVertice(-10, 55, blue), GVertice(60, 50, blue), GVertice(60, 55, blue));
    drawSquare(this, GVertice(5, 0, blue), GVertice(5, 10, blue), GVertice(15, 0, blue), GVertice(15, 10, blue));
    drawSquare(this, GVertice(35, 0, blue), GVertice(35, 10, blue), GVertice(45, 0, blue), GVertice(45, 10, blue));
    
    if(colisionBox != NULL) {
        GShape *colisionBox = createRectangle(-8, 0, 60, 54);
        this->colisionBox = colisionBox;
    }
}
