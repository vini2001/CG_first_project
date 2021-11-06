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
#include"VAO.hpp"
#include"VBO.hpp"
#include"EBO.hpp"

#include <stb/stb_image.h>
#include "GShape.hpp"
#include "GObject.hpp"
#include "customObjects.h"
#include "GStack.hpp"
#include <chrono>
#include <map>

using namespace std;
 

const int width = 800;
const int height = 800;


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
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    switch (key) {
        case GLFW_KEY_ESCAPE:
        case GLFW_PRESS:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        default:
            if(!(find(supportedKeys.begin(), supportedKeys.end(), key) != supportedKeys.end())) break;

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


int main(void){
    
    
    vector<GObject*> objects;

    
    GStack *st = createSpaceShip(-0.3f, -0.9f, 0.7f);
    objects.push_back(st);
    
    
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    
    GLFWwindow* window = glfwCreateWindow(width, height, "TP1 Computação Gráfica", NULL, NULL);
    if(window == NULL) {
        cout << "Failed to create window" << endl;
        glfwTerminate();
        return 0;
    }
    
    glfwSetKeyCallback(window, key_callback);
    
    glfwMakeContextCurrent(window);
    gladLoadGL();
    
    Shader shaderProgram("shaders/default.vert", "shaders/default.frag");

    
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->prepare();
    }
    
    
    GLuint scaleUniID = glGetUniformLocation(shaderProgram.ID, "scale");
    
    long lastTime = getMillis();
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.activate();
        glUniform1f(scaleUniID, 0.0f);
    
        long timeEllapsed = getMillis() - lastTime;
        if(timeEllapsed > 5) {
            if(pressedKey == GLFW_KEY_LEFT) {
                vec2 change = {  -0.02f, 0.0f };
                st->addPos(change);
                st->prepare();
            }else if(pressedKey == GLFW_KEY_RIGHT) {
                vec2 change = { 0.02f, 0.0f };
                st->addPos(change);
                st->prepare();
            }
            lastTime = getMillis();
        }
        
        for(int i = 0; i < objects.size(); i++) {
            objects[i]->draw(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    shaderProgram.deleteIt();
    delete st;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
