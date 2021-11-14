#ifndef GAlienSpaceship_hpp
#define GAlienSpaceship_hpp

#define ALIEN_NO_PRIZE 0
#define ALIEN_SHOOTING_PRIZE 1
#define ALIEN_SHOOTING_INTERVAL_PRIZE 2
#define ALIEN_SHOOTING_SPEED_PRIZE 3

#include <stdio.h>
#include "GShape.hpp"

class GAlien : public GShape {
public:
    int prize = ALIEN_NO_PRIZE;
    int life = 100;
    GAlien(GLfloat x, GLfloat y, int prize);
    void refresh();
    
    int row, pos;
};

#endif /* GAlienSpaceship_hpp */
