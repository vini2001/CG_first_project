#include "gameController.hpp"

GameController::GameController() {}


void GameController::init(){
    player = createSpaceShip(-100, -game::height/2 * 0.95, Vec2(0.35, 0.35));
    addObject(player);
    playerAlive = true;
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
                    continue;
                }
            }
        }
    }
}
