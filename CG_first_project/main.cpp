#include <iostream>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>

#include "shaderClass.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

#include <stb/stb_image.h>
#include "GShape.hpp"
#include "GObject.hpp"
#include "customObjects.h"
#include "GStack.hpp"
#include <chrono>
#include <map>
#include "globals.hpp"

using namespace std;



GLuint indices[] = {0, 1, 2,  0, 2, 3 };

float getRand() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

long getMillis() {
    return chrono::duration_cast<std::chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}


static GLuint pressedKey;
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

bool shoot = false;
void mouse_button_callback(GLFWwindow* windowGame, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        shoot = true;
    }
}

vector<GObject*> objects;

void WindowSizeCallback(GLFWwindow* window, int width, int height){
    game::width = width;
    game::height = height;
    
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->prepare();
    }
}


void fire(GStack *spaceShip) {
    float* spawnPos = spaceShip->getItemSpawnPos();
    GShape *bullet = createRectangle(spawnPos[0] - 1.5, spawnPos[1] - 100, 3, 30);
    bullet->setSpeed(0.0, 40.0);
    bullet->destroyAt = getMillis() + 500;
    objects.push_back(bullet);
}


int main(void){
    
        
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_REFRESH_RATE, 4);
    
    
    
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
    
    Shader shaderProgram("shaders/default.vert", "shaders/default.frag");
    
    
    GStack *spaceShip = createSpaceShip(-100, -game::height/2 * 0.98, new vec2 {0.34, 0.34});
    objects.push_back(spaceShip);

    
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->prepare();
    }
    
    
    
    GLuint scaleUniID = glGetUniformLocation(shaderProgram.ID, "scale");
    
    long lastTime = getMillis();
    while (!glfwWindowShouldClose(game::window)) {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.activate();
        glUniform1f(scaleUniID, 0.0f);
    
        long timeEllapsed = getMillis() - lastTime;
        if(timeEllapsed > 5) {
            
            if(pressedKey == GLFW_KEY_LEFT) {
                GLfloat toBorder = -game::width/2 - spaceShip->x;
                vec2 change = { (spaceShip->x - 20 < -game::width/2 ? toBorder : -20.f), 0.0f };
                spaceShip->addPos(change);
            }else if(pressedKey == GLFW_KEY_RIGHT) {
                GLfloat rightmostX = spaceShip->x + spaceShip->boxSize[0];
                GLfloat toBorder = game::width/2 - rightmostX;
                vec2 change = { (rightmostX + 20 > game::width/2 ? toBorder : 20.f), 0.0f };
                spaceShip->addPos(change);
            }
            lastTime = getMillis();
        }
        
        if(shoot) {
            fire(spaceShip);
            shoot = false;
        }
        
        for(int i = 0; i < objects.size(); i++) {
            if(objects[i]->shouldDestroy(getMillis())) {
                GShape* c = dynamic_cast<GShape*>(objects[i]);
                delete c;
                objects.erase(objects.begin()+i);
                i--;
                continue;
            }
            objects[i]->update();
            objects[i]->draw(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        }
        
        glfwSwapBuffers(game::window);
        glfwPollEvents();
    }
    
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->destroy();
        free(objects[i]);
    }
    
    shaderProgram.deleteIt();
    glfwDestroyWindow(game::window);
    glfwTerminate();
    return 0;
}
