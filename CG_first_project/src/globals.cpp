#include "globals.hpp"
#include <stdio.h>


int game::width;
int game::height;
int game::shootingInterval = 300;
bool game::colisionsEnabled = true;
GLFWwindow* game::window;

int debugLevel;
long framesSinceRender;
