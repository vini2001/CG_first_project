#include "globals.hpp"


int game::width;
int game::height;
bool game::colisionsEnabled = true;
bool game::started = false;
long game::pausedTime = 0;
long game::pausedAt = 0;
bool game::paused = false;
long game::frameCount = 0;

GLFWwindow* game::window;

int debugLevel;
long framesSinceUpdate;
