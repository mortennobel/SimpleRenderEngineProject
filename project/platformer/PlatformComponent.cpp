//
// Created by Morten Nobel-Jørgensen on 11/4/17.
//

#include <Box2D/Box2D.h>
#include <iostream>
#include <glm/ext.hpp>
#include "PlatformComponent.hpp"
#include "PlatformerGame.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"

PlatformComponent::PlatformComponent(GameObject *gameObject) : Component(gameObject) {
    auto game = PlatformerGame::instance;
    physicsScale = game->physicsScale;
}

void PlatformComponent::init(std::shared_ptr<sre::SpriteAtlas> spriteAtlas, int x, int y, int startSpriteId, int width, bool kinematic) {
    this->kinematic = kinematic;
    this->width = width;
    auto game = PlatformerGame::instance;
    auto spriteComponent = gameObject->addComponent<SpriteComponent>();
    auto sprite = spriteAtlas->get(std::to_string(startSpriteId)+".png");
    float tileSize = Level::tileSize;
    glm::vec2 offset{tileSize/2,tileSize/2};
    glm::vec2 pos = offset + glm::vec2{x*tileSize,y*tileSize};

    gameObject->setPosition(pos);

    this->pos = pos;

    spriteComponent->setSprite(sprite);

    physics = gameObject->addComponent<PhysicsComponent>();

    pos.x += (width-1)*0.5f*tileSize;
    physics->initBox(kinematic?b2_kinematicBody:b2_staticBody, glm::vec2{tileSize*width,tileSize}/physicsScale*0.5f,pos/physicsScale,0);

    physics->setAutoUpdate(false);
    for (int i = 1;i<width;i++){
        auto go = game->createGameObject();
        auto spriteComponent = go->addComponent<SpriteComponent>();
        int spriteOffset = i==width-1?2:1; // select middle or right piece
        auto sprite = spriteAtlas->get(std::to_string(startSpriteId+spriteOffset)+".png");
        glm::vec2 offset{tileSize/2,tileSize/2};
        glm::vec2 pos = offset + glm::vec2{(x+i)*tileSize,y*tileSize};
        go->setPosition(pos);
        spriteComponent->setSprite(sprite);
        tiles.push_back(go);
    }
}

void PlatformComponent::initWall(std::shared_ptr<sre::SpriteAtlas> spriteAtlas, int x, int y, int startSpriteId, int height) {
    this->kinematic = false; // walls cannot be moved
    auto game = PlatformerGame::instance;

    auto spriteComponent = gameObject->addComponent<SpriteComponent>();
    auto sprite = spriteAtlas->get(std::to_string(startSpriteId+30)+".png");
    float tileSize = Level::tileSize;
    glm::vec2 offset{tileSize/2,tileSize/2};
    pos = offset + glm::vec2{x*tileSize,y*tileSize};

    gameObject->setPosition(pos);

    spriteComponent->setSprite(sprite);
    physics = gameObject->addComponent<PhysicsComponent>();
    pos.y += (height-1)*0.5f*tileSize;
    physics->initBox(kinematic?b2_kinematicBody:b2_staticBody, glm::vec2{tileSize,tileSize*height}/physicsScale*0.5f,pos/physicsScale,0);
    physics->setAutoUpdate(false);

    for (int i = 1;i<height;i++){
        auto go = game->createGameObject();
        auto spriteComponent = go->addComponent<SpriteComponent>();
        int spriteOffset = i==height-1?0:30; // select middle or right piece
        auto sprite = spriteAtlas->get(std::to_string(startSpriteId+spriteOffset)+".png");
        glm::vec2 offset{tileSize/2,tileSize/2};
        glm::vec2 pos = offset + glm::vec2{x*tileSize,(y+i)*tileSize};
        go->setPosition(pos);
        spriteComponent->setSprite(sprite);

        tiles.push_back(go);
    }
}


void PlatformComponent::moveTo(glm::vec2 tilePos) {
    assert(kinematic);
    glm::vec2 offset{Level::tileSize/2,Level::tileSize/2};
    pos = offset + glm::vec2{tilePos.x*Level::tileSize,tilePos.y*Level::tileSize};
    this->pos = pos;

    gameObject->setPosition(pos);

    glm::vec2 physPos = pos;
    physPos.x += (width-1)*0.5f*Level::tileSize;
    physics->moveTo(physPos/physicsScale);

    for (int i=0;i<tiles.size();i++){
        pos.x += Level::tileSize;
        tiles[i]->setPosition(pos);
    }
}

glm::vec2 PlatformComponent::getPosition() {
    return pos;
}

void PlatformComponent::update(float deltaTime) {
    if (kinematic){
        glm::vec2 physPos = pos;
        physPos.x += (width-1)*0.5f*Level::tileSize;
        physics->moveTo(physPos/physicsScale);
    }
}
