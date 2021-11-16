#include "gameController.hpp"
#include "utils.hpp"
#include <iostream>
#include "GText.hpp"
#include "GAlienSpaceship.hpp"


GameController::GameController() {}


void GameController::init(Shader *shaderProgram){
    this->shader = shaderProgram;
    player = createSpaceShip(-100, -game::height/2 * 0.95, Vec2(0.35, 0.35));
    addObject(player);
    playerAlive = true;
    
    // create a bunch of aliens
    for(int r = 0; r < 15; r++) {
        float alienSize = 0.65;
        for(int i = 0 + (r < 7 ? 0 : r - 7); i < 15-(r < 7 ? 0 : r - 7); i++) {
            
            int prize = getRand() > 0.94 ? ALIEN_SHOOTING_PRIZE : ALIEN_NO_PRIZE;
            if(getRand() > 0.94) prize = ALIEN_SHOOTING_INTERVAL_PRIZE;
            if(getRand() > 0.94) prize = ALIEN_SHOOTING_SPEED_PRIZE;
            GAlien *alien = new GAlien(240 + (120*alienSize)*i - game::width/2, game::height/2 + 500 - (90*alienSize)*r, prize);
            alien->setLabel("alien");
            alien->setSpeed(Vec2(0, -0.14));
            alien->setScale(Vec2(alienSize, alienSize));
            addObject(alien);
            
            alien->row = r;
            alien->pos = i;
            
            
            aliens.push_back(alien);
        }
    }
    
    gText = new GText();
    lastAttack = getMillis();
    game::started = true;
}

void GameController::handleInput(GLuint pressedKey, GLuint pressedMouseButton, Vec2 mousePos) {
    
    if(pressedKey == GLFW_KEY_R) {
        destroy();
        init(shader);
    }
    
    if(!game::started) return;
    
    float playerXpos = player->x + player->boxSize.x/2;
    float spaceShipSpeed = abs(mousePos.x - playerXpos)/5;
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
    
    
    if (pressedMouseButton == GLFW_MOUSE_BUTTON_LEFT || pressedKey == GLFW_KEY_S) {
        shoot = true;
    }else if(pressedMouseButton == GLFW_MOUSE_BUTTON_RIGHT) {
        game::paused = !game::paused;
        if(game::paused) {
            game::pausedAt = getRealMillis();
            flashMessages.push_back(FlashMessage("PAUSED", game::width/2 - 90, game::height/2, glm::vec3(1, 1, 1), getMillis() + 1, 1));
        }else{
            game::pausedTime += getRealMillis() - game::pausedAt;
        }
    }
}

void GameController::fire(GStack *spaceShip) {
    Vec2 spawnPos = spaceShip->getItemSpawnPos();
    
    bool b1 = false, b2 = false, b3 = false;
    switch(playerBulletsLevel) {
        case 1:
            b1 = true;
            break;
        case 2:
            b2 = b3 = true;
            break;
        case 3: b1 = b2 = b3 = true;
            break;
    }
    
    
    if(b1) {
        GStack *bullet1 = createBullet(spawnPos.x - 1, spawnPos.y);
        bullet1->destroyAt = getMillis() + 10000/(int)bulletsSpeed;
        bullet1->setLabel("bullet");
        bullet1->setSpeed(Vec2(0.0, bulletsSpeed));
        // insert at the beggining so the ship will always be render over the bullet
        objects.insert(objects.begin(), bullet1);
    }

    if(b2) {
        GStack *bullet2 = createBullet(spawnPos.x - 1, spawnPos.y);
        bullet2->destroyAt = getMillis() + 10000/(int)bulletsSpeed;
        bullet2->setLabel("bullet");
        bullet2->setSpeed(Vec2(-bulletsSpeed/10, bulletsSpeed));
        // insert at the beggining so the ship will always be render over the bullet
        objects.insert(objects.begin(), bullet2);
    }
    
    if(b3) {
        GStack *bullet3 = createBullet(spawnPos.x - 1, spawnPos.y);
        bullet3->destroyAt = getMillis() + 10000/(int)bulletsSpeed;
        bullet3->setLabel("bullet");
        bullet3->setSpeed(Vec2(bulletsSpeed/10, bulletsSpeed));
        objects.insert(objects.begin(), bullet3);
    }
}


void GameController::frameActions() {
    if(game::paused) return;
    
    if(shoot && lastShot + shootingInterval < getMillis() && playerAlive) {
        lastShot = getMillis();
        fire(player);
        shoot = false;
    }
    
    if(changeDirAt < getMillis()) {
        goingEsq = !goingEsq;
        changeDirAt = getMillis() + 1000;
    }
    
    for(int i = 0; i < objects.size(); i++) {

        if(objects[i]->getLabel() == "alien") {
            objects[i]->setSpeed(Vec2(goingEsq ? -1.5 : 1.5, objects[i]->getSpeed().y));
            
            if(objects[i]->y + 50 < -game::height/2) {
                // TODO: this code repeats a bit, may need refactoring
                objects[i]->destroyAt = 0;
                deleteFromAliensArray(objects[i]);
                vector<GObject*>::iterator it = find(objects.begin(), objects.end(), objects[i]);
                delete dynamic_cast<GShape*>(objects[i]);
                if(it != objects.end()) {
                    objects.erase(it); i--;
                }
                
                if(game::started) {
                    game::started = false;
                    flashMessages.push_back(FlashMessage("The Earth has been destroyed!!!", game::width/2 - 300, game::height/2, glm::vec3(0.9, 0.05, 0.2), getMillis() + 20000, 1));
                }
            }
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
    
    long sinceLastAttack = getMillis() - lastAttack;
    if(sinceLastAttack > 6000 && aliens.size() > 0) {
        lastAttack = getMillis();
        
        int curRow = -1;
        int qtt = 0;
        for(int i = (int)aliens.size()-1; i > 0; i--) {
            if(curRow == -1 || aliens[i]->row == curRow) {
                qtt ++;
                curRow = aliens[i]->row;
            }else{
                break;
            }
        }
        
        int sort = round(getRand() * (qtt-1)) + 1;
        int indexAttack = (int)aliens.size() - sort;
        aliens[indexAttack]->setSpeed(Vec2(0, -1.6));
        
        // if there is a close alien to the right, there is a chance this one will go with the first one
        if(indexAttack + 1 < aliens.size() && aliens[indexAttack+1]->row == curRow && getRand() > 0.6) {
            aliens[indexAttack + 1]->setSpeed(Vec2(0, -1.6));
        }
    }
}

void GameController::drawElements() {
    
//    if(game::started) {
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
//    }
    
    
    for(int i = 0; i < flashMessages.size(); i++) {
        if(flashMessages[i].finishAt > getMillis()) {
            FlashMessage m = flashMessages[i];
            drawText(m.text, m.x, 70 * i + m.y, flashMessages[i].scale, m.rgb);
        }else{
            flashMessages.erase(flashMessages.begin()+i--);
        }
    }
}

void GameController::addObject(GObject *obj){
    objects.push_back(obj);
}

void GameController::detectColisions() {
    if(game::colisionsEnabled){
        for(int i = 0; i < objects.size(); i++) {
            if(objects[i]->getLabel() == "alien") {

                GAlien *alien = dynamic_cast<GAlien*>(objects[i]);
                GObject* colidedWith = alien->testColision(objects, "spaceship");
                if(colidedWith != NULL) {
                    player->destroyAt = 0;
                    playerAlive = false;
                    deleteObjectFromObjects(player); i--;
                    if(game::started) {
                        game::started = false;
                        flashMessages.push_back(FlashMessage("WASTED", game::width/2 - 250, game::height/2, glm::vec3(0.9, 0.05, 0.2), getMillis() + 20000, 2.5));
                    }
                }

                colidedWith = alien->testColision(objects, "bullet");
                if(colidedWith != NULL) {
                    colidedWith->setSpeed(Vec2(0.0, 0.0));
                    colidedWith->destroyAt = 0;
                    
                    GAlien *alien = dynamic_cast<GAlien*>(objects[i]);
                    alien->life -= 34 * ((5+(bulletsSpeed-5)/2)/5);
                    
                    if(alien->life <= 0) {
                        alien->destroyAt = 0;
                        if(alien->prize == ALIEN_SHOOTING_PRIZE) {
                            if(playerBulletsLevel < 3) {
                                playerBulletsLevel++;
                                flashMessages.push_back(FlashMessage("Upgrade: 1 more bullet!", 50, game::height-50, glm::vec3(0.3, 1, 0.3), getMillis() + 1000));
                            }
                        }else if(alien->prize == ALIEN_SHOOTING_INTERVAL_PRIZE) {
                            if(shootingInterval < 150) {
                                shootingInterval = 150;
                            }else{
                                shootingInterval *= 0.8;
                                flashMessages.push_back(FlashMessage("Upgrade: more shots per seconds!", 50, game::height-50, glm::vec3(0.3, 1, 0.3), getMillis() + 1000));
                            }
                        }
                        else if(alien->prize == ALIEN_SHOOTING_SPEED_PRIZE) {
                            if(bulletsSpeed > 16) {
                                bulletsSpeed = 16;
                            }else{
                                bulletsSpeed += 2;
                                flashMessages.push_back(FlashMessage("Upgrade: faster shots!", 50, game::height-50, glm::vec3(0.3, 1, 0.3), getMillis() + 1000));
                            }
                        }
                        
                        deleteFromAliensArray(objects[i]);
                        
                        delete dynamic_cast<GShape*>(objects[i]);
                        objects.erase(objects.begin()+i--);
                    }else{
                        alien->refresh();
                        if(alien->row == aliens[aliens.size()-1]->row) {
                            alien->setSpeed(Vec2(0, alien->getSpeed().y*1.3));
                        }
                    }
                    
                    deleteObjectFromObjects(colidedWith); i--;
                    continue;
                }
            }
        }
    }
}

void GameController::deleteObjectFromObjects(GObject *object) {
    vector<GObject*>::iterator it = find(objects.begin(), objects.end(), object);
    if(it != objects.end()) {
        int indexBullet = (int) distance(objects.begin(), it);
        delete dynamic_cast<GStack*>(objects[indexBullet]);
        objects.erase(it);
    }
}

void GameController::deleteFromAliensArray(GObject* alienObj) {
    GAlien *alien = dynamic_cast<GAlien*>(alienObj);
    vector<GAlien*>::iterator it = find(aliens.begin(), aliens.end(), alien);
    if(it != aliens.end()) {
        aliens.erase(it);
    }
}

void GameController::drawText(string text, float x, float y, float scale, glm::vec3 colors){
    gText->renderText(text, x, y, scale, colors);
    shader->activate();
}

void GameController::destroy() {
    delete gText;
    flashMessages.clear();
    playerBulletsLevel = 1;
    bulletsSpeed = 5.0;
    shootingInterval = 300;
    
    for(int i = 0; i < objects.size(); i++) {
        if(objects[i]->classType == "shape") {
            delete dynamic_cast<GShape*>(objects[i]);
        }else if(objects[i]->classType == "stack") {
            delete dynamic_cast<GStack*>(objects[i]);
        }else{
            objects[i]->destroy();
            free(objects[i]);
        }
    }
    
    aliens.clear();
    objects.clear();
}

GameController::~GameController() {
    destroy();
}
