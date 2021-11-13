#include "gameController.hpp"
#include "utils.hpp"
#include <iostream>
#include "GText.hpp"


GameController::GameController() {}


void GameController::init(){
    player = createSpaceShip(-100, -game::height/2 * 0.95, Vec2(0.35, 0.35));
    addObject(player);
    playerAlive = true;
    
    // create a bunch of aliens
    for(int r = 0; r < 6; r++) {
        for(int i = 0+r; i < 11-r; i++) {
            GShape *alien = createAlien(320 + 85*i - game::width/2, game::height/2 - 100 - 70*r);
            alien->setLabel("alien");
            alien->setSpeed(Vec2(0, -0.05));
            addObject(alien);
        }
    }
    
    gText = new GText();
}

void GameController::handleInput(GLuint pressedKey, GLuint pressedMouseButton, Vec2 mousePos) {
    float playerXpos = player->x + player->boxSize.x/2;
    float spaceShipSpeed = abs(mousePos.x - playerXpos)/20;
    if(mousePos.x < playerXpos - 15) {
        GLfloat toBorder = -game::width/2 - player->x - player->boxSize.x/2;
        Vec2 change((player->x - spaceShipSpeed + player->boxSize.x/2 < -game::width/2 ? toBorder : -spaceShipSpeed), 0.0f);
        player->setSpeed(change);
    }else if(mousePos.x > playerXpos + 15) {
        GLfloat rightmostX = player->x + player->boxSize.x*0.5;
        GLfloat toBorder = game::width/2 - rightmostX;
        Vec2 change = { (rightmostX + spaceShipSpeed > game::width/2 ? toBorder : spaceShipSpeed), 0.0f };
        player->setSpeed(change);
    }else{
        player->setSpeed(Vec2(0, 0));
    }
    
    if (pressedMouseButton == GLFW_MOUSE_BUTTON_LEFT) {
        shoot = true;
    }
}

void GameController::fire(GStack *spaceShip) {
    Vec2 spawnPos = spaceShip->getItemSpawnPos();
    
    float bulletsSpeed = 5.0;
    
    GStack *bullet1 = createBullet(spawnPos.x - 1, spawnPos.y);
    bullet1->destroyAt = getMillis() + 10000/(int)bulletsSpeed;
    bullet1->setLabel("bullet");
    bullet1->setSpeed(Vec2(0.0, bulletsSpeed));
    // insert at the beggining so the ship will always be render over the bullet
    objects.insert(objects.begin(), bullet1);

    GStack *bullet2 = createBullet(spawnPos.x - 1, spawnPos.y);
    bullet2->destroyAt = getMillis() + 10000/(int)bulletsSpeed;
    bullet2->setLabel("bullet");
    bullet2->setSpeed(Vec2(-bulletsSpeed/10, bulletsSpeed));
    // insert at the beggining so the ship will always be render over the bullet
    objects.insert(objects.begin(), bullet2);
    
    GStack *bullet3 = createBullet(spawnPos.x - 1, spawnPos.y);
    bullet3->destroyAt = getMillis() + 10000/(int)bulletsSpeed;
    bullet3->setLabel("bullet");
    bullet3->setSpeed(Vec2(bulletsSpeed/10, bulletsSpeed));
    objects.insert(objects.begin(), bullet3);
}


void GameController::frameActions() {
    if(shoot && lastShot + game::shootingInterval < getMillis() && playerAlive) {
        lastShot = getMillis();
        fire(player);
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
    }
    
    detectColisions();
}

void GameController::drawElements() {
    
    if(vao != NULL) delete vao;
    
    vao = new VAO();
    vao->bind();
    int sizeVArray = 0;
    int sizeIArray = 0;
    int trianglesQuantity = 0;
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->getSizes(sizeVArray, sizeIArray, trianglesQuantity);
    }
    
    if(vArray != NULL) { free(vArray); vArray = NULL; }
    if(indices != NULL) { free(indices); indices = NULL; }
    
    vArray = (GLfloat*) malloc(sizeVArray);
    indices = (GLuint*) malloc(sizeIArray);
    long arrayPos = 0;
    long indicesPos = 0;
    
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->prepare(vArray, arrayPos, indices, indicesPos);
    }
    
    vbo1 = new VBO(vArray, (int) sizeVArray);
    
    ebo1 = new EBO(indices, (int) sizeIArray);
    
    vao->linkAttrib(*vbo1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    vao->linkAttrib(*vbo1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    
    vao->unbind();
    delete vbo1;
    delete ebo1;
    
    vao->bind();
    glDrawElements(GL_TRIANGLES, trianglesQuantity*3, GL_UNSIGNED_INT, 0);
}

void GameController::addObject(GObject *obj){
    objects.push_back(obj);
}

void GameController::detectColisions() {
    if(game::colisionsEnabled){
        for(int i = 0; i < objects.size(); i++) {
            if(objects[i]->getLabel() == "alien") {

                GObject* colidedWith = objects[i]->testColision(objects, "spaceship");
                if(colidedWith != NULL) {
                    player->destroyAt = 0;
                    playerAlive = false;
                }

                colidedWith = objects[i]->testColision(objects, "bullet");
                if(colidedWith != NULL) {
                    colidedWith->setSpeed(Vec2(0.0, 0.0));
                    objects[i]->destroyAt = 0;
                    colidedWith->destroyAt = 0;
                    delete dynamic_cast<GShape*>(objects[i]);
                    objects.erase(objects.begin()+i--);
                    
                    vector<GObject*>::iterator it = find(objects.begin(), objects.end(), colidedWith);
                    if(it != objects.end()) {
                        int indexBullet = (int) distance(objects.begin(), it);
                        delete dynamic_cast<GStack*>(objects[indexBullet]);
                        objects.erase(it); i--;
                    }
                    continue;
                }
            }
        }
    }
}

void GameController::drawText(string text, float x, float y, float scale, glm::vec3 colors){
    gText->renderText(text, x, y, scale, colors);
}

void GameController::destroy() {
    for(int i = 0; i < objects.size(); i++) {
        objects[i]->destroy();
        free(objects[i]);
    }
}


GameController::~GameController() {
    delete gText;
}
