#include "GStar.hpp"
#include "customObjects.h"
#include "utils.hpp"

GStar::GStar(GLfloat x, GLfloat y) : GShape(x, y) {
    float width = 5 + getRand()*10;
    float height = 2 + getRand()*10;
    
    GLfloat r = 1.0f, g = 1.0f, b = 1.0f;
    GVertice v1 = GVertice(0.00f, 0.00f, getRand()/4, getRand()/4, getRand()/4);
    GVertice v2 = GVertice(width, 0.00f, 0, 0, 0);
    GVertice v3 = GVertice(width/2, height, r/3, g/3, b/3);
    drawTriangle(this, v1, v2, v3);
}


bool GStar::update() {
    GShape::update();
    if(y < -game::height/2) {
        y += game::height + 10;
        x = getRand() * game::width - game::width/2;
    }
    return true;
}
