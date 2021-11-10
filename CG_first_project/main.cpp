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
    Vec2 spawnPos = spaceShip->getItemSpawnPos();
    
    float bulletsSpeed = 5.0;
    
    GStack *bullet1 = createBullet(spawnPos.x - 1, spawnPos.y);
    bullet1->destroyAt = getMillis() + 2000;
    bullet1->setLabel("bullet");
    bullet1->setSpeed(Vec2(0.0, bulletsSpeed));
    // insert at the beggining so the ship will always be render over the bullet
    objects.insert(objects.begin(), bullet1);

    GStack *bullet2 = createBullet(spawnPos.x - 1, spawnPos.y);
    bullet2->destroyAt = getMillis() + 2000;
    bullet2->setLabel("bullet");
    bullet2->setSpeed(Vec2(-bulletsSpeed/10, bulletsSpeed));
    // insert at the beggining so the ship will always be render over the bullet
    objects.insert(objects.begin(), bullet2);
    
    GStack *bullet3 = createBullet(spawnPos.x - 1, spawnPos.y);
    bullet3->destroyAt = getMillis() + 2000;
    bullet3->setLabel("bullet");
    bullet3->setSpeed(Vec2(bulletsSpeed/10, bulletsSpeed));
    objects.insert(objects.begin(), bullet3);
    
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
    
    GStack *spaceShip = createSpaceShip(-100, -game::height/2 * 0.95, Vec2(0.35, 0.35));
    objects.push_back(spaceShip);
    
    
    int countAliens = 0;
    
    // create a bunch of aliens
    for(int r = 0; r < 6; r++) {
        for(int i = 0+r; i < 11-r; i++) {
            countAliens ++;
            GShape *alien = createAlien(320 + 85*i - game::width/2, game::height/2 - 100 - 70*r);
            alien->setLabel("alien");
            alien->setSpeed(Vec2(0, -0.5));
            objects.push_back(alien);
        }
    }
    
    cout << countAliens << " aliens created " << endl;
    
    
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->prepare();
    }
    
    long changeDirAt = 0;
    bool goingEsq = false;
    
    vector<int> fpsV = {60, 60, 60};
    int fpsCount = 0;
    int fpsSum = 0;
    
    long lastTime = getMillis();
    long lastShoot = 0;
    while (!glfwWindowShouldClose(game::window)) {
        
        long timeEllapsed = getMillis() - lastTime;
        
        if(timeEllapsed > 10) {
            
            framesSinceRender = timeEllapsed;
            
            fpsV.erase(fpsV.begin()+0);
            fpsV.push_back(1000/timeEllapsed);
            int fps = (fpsV[0]+fpsV[1]+fpsV[2])/3;
            fpsCount++; fpsSum += fps;
            
            if(fps < 60) {
                cout << "FPS: " << fps  << " (Avg: " << fpsSum/fpsCount << ")" << endl;
            }
            
            glClearColor(0.03f, 0.06f, 0.08f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            shaderProgram.activate();
            
            float spaceShipSpeed = 10;
            
            if(pressedKey == GLFW_KEY_LEFT) {
                GLfloat toBorder = -game::width/2 - spaceShip->x - spaceShip->boxSize.x/2;
                Vec2 change((spaceShip->x - spaceShipSpeed + spaceShip->boxSize.x/2 < -game::width/2 ? toBorder : -spaceShipSpeed), 0.0f);
                spaceShip->setSpeed(change);
            }else if(pressedKey == GLFW_KEY_RIGHT) {
                GLfloat rightmostX = spaceShip->x + spaceShip->boxSize.x*0.5;
                GLfloat toBorder = game::width/2 - rightmostX;
                Vec2 change = { (rightmostX + spaceShipSpeed > game::width/2 ? toBorder : spaceShipSpeed), 0.0f };
                spaceShip->setSpeed(change);
            }else{
                spaceShip->setSpeed(Vec2(0, 0));
            }
            lastTime = getMillis();
            
            if(shoot && lastShoot + game::shootingInterval < getMillis()) {
                lastShoot = getMillis();
                fire(spaceShip);
                shoot = false;
            }
            
            
            if(changeDirAt < getMillis()) {
                goingEsq = !goingEsq;
                changeDirAt = getMillis() + 500;
            }
            
            for(int i = 0; i < objects.size(); i++) {

                if(objects[i]->getLabel() == "alien") {
                    objects[i]->setSpeed(Vec2(goingEsq ? -3 : 3, objects[i]->getSpeed().y));
                }

                if(objects[i]->shouldDestroy(getMillis())) {
                    GStack* c = dynamic_cast<GStack*>(objects[i]);
                    delete c;
                    objects.erase(objects.begin()+i);
                    i--;
                    continue;
                }
                objects[i]->update();
                objects[i]->draw(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);
            }
            
            
            if(game::colisionsEnabled){
                for(int i = 0; i < objects.size(); i++) {
        
                    if(objects[i]->getLabel() == "alien") {
        
                        GObject* colidedWith = objects[i]->testColision(objects, "spaceship");
                        if(colidedWith != NULL) {
                            spaceShip->destroyAt = 0;
                        }
        
                        colidedWith = objects[i]->testColision(objects, "bullet");
                        if(colidedWith != NULL) {
                            colidedWith->setSpeed(Vec2(0.0, 0.0));
                            objects[i]->destroyAt = 0;
                            colidedWith->destroyAt = 0;
                            delete dynamic_cast<GShape*>(objects[i]);
                            objects.erase(objects.begin()+i--);
                            continue;
                        }
                    }
                }
            }
            
            glfwSwapBuffers(game::window);
            glfwPollEvents();
        }
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
