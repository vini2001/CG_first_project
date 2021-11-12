#include <iostream>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include "utils.hpp"

#include "shaderClass.hpp"

#include <stb/stb_image.h>
#include "GShape.hpp"
#include "GObject.hpp"
#include "customObjects.h"
#include "GStack.hpp"
#include <map>
#include "globals.hpp"
#include "gameController.hpp"

using namespace std;


float getRand() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

GameController gameController;

static GLuint pressedKey;
static GLuint pressedMouseButton = -1;
static map<GLuint, bool> keyIsPressed;

vector<GLuint> supportedKeys = { GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_ESCAPE, GLFW_PRESS };
static bool holdingKey;
static void key_callback(GLFWwindow* windowGame, int key, int scancode, int action, int mods){
    switch (key) {
        case GLFW_KEY_ESCAPE:
        case GLFW_PRESS:
            glfwSetWindowShouldClose(windowGame, GLFW_TRUE);
            break;
        default:
            if(!(find(supportedKeys.begin(), supportedKeys.end(), key) != supportedKeys.end())) {
                cout << "unsupported key " << key << endl;
                break;
            }

            if(pressedKey == key && action == GLFW_RELEASE) {
                pressedKey = NULL;
                keyIsPressed[pressedKey] = false;
            }else if(action == GLFW_PRESS || action == GLFW_REPEAT){
                pressedKey = key;
                keyIsPressed[pressedKey] = true;
                holdingKey = action == GLFW_REPEAT;
            }
            break;
    }
}

void mouse_button_callback(GLFWwindow* windowGame, int button, int action, int mods){
    if (action == GLFW_PRESS) {
        pressedMouseButton = button;
    }else{
        pressedMouseButton = -1;
    }
}

void WindowSizeCallback(GLFWwindow* window, int width, int height){
    game::width = width;
    game::height = height;
    
    gameController.drawElements();
}

int main(void){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_REFRESH_RATE, 15);
    
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
     
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    
    game::window = glfwCreateWindow(3000, 3000, "CG game", NULL, NULL);
    if(game::window == NULL) {
        cout << "Failed to create windowGame" << endl;
        glfwTerminate();
        return 0;
    }
    
    glfwSetKeyCallback(game::window, key_callback);
    glfwSetMouseButtonCallback(game::window, mouse_button_callback);
    glfwSetWindowSizeCallback(game::window, WindowSizeCallback);
    glfwGetWindowSize(game::window, &game::width, &game::height);
    
    cout << "height: " << game::height << ", width: " << game::width << endl;
    
    glfwMakeContextCurrent(game::window);
    gladLoadGL();
    
    Shader shaderProgram("default.vert", "default.frag");
    gameController.init();

    
    vector<int> fpsV = {60, 60, 60};
    int fpsCount = 0;
    int fpsSum = 0;
    
    long lastTime = getMillis();
    while (!glfwWindowShouldClose(game::window)) {
        
        long timeEllapsed = getMillis() - lastTime;
        if(timeEllapsed > 10) {
            lastTime = getMillis();
            framesSinceRender = timeEllapsed;
            
            fpsV.erase(fpsV.begin()+0);
            fpsV.push_back(1000/timeEllapsed);
            int fps = (fpsV[0]+fpsV[1]+fpsV[2])/3;
            fpsCount++; fpsSum += fps;
            
//            cout << "FPS: " << fps  << " (Avg: " << fpsSum/fpsCount << ")" << endl;
            
            glClearColor(0.03f, 0.06f, 0.08f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            shaderProgram.activate();
            
            gameController.handleInput(pressedKey, pressedMouseButton);
            pressedMouseButton = -1; // set as -1 to avoid duplicated actions
            gameController.frameActions();
            gameController.drawElements();
            
            glfwSwapBuffers(game::window);
            glfwPollEvents();
        }
    }
    
    gameController.destroy();
    
    shaderProgram.deleteIt();
    glfwDestroyWindow(game::window);
    glfwTerminate();
    return 0;
}
