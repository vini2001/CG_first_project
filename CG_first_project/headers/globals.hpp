#ifndef globals_hpp
#define globals_hpp

#include <stdio.h>
#include <GLFW/glfw3.h>

#define DEBUG_ALL 1

namespace game {
    extern GLFWwindow *window;
    extern int width, height;
}
extern int debugLevel;

#endif /* globals_hpp */
