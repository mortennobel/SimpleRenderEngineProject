//
// Created by Morten Nobel Jørgensen on 2018-10-29.
//

#include "RPG.h"
#include "Component.hpp"
#include "Hero.hpp"
#include "MainMenuComponent.h"
#include "SpriteComponent.hpp"
#include "sre/SpriteAtlas.hpp"

using namespace sre;

RPG::RPG()
:currentScene(&mainMenu)
{
    r.setWindowSize({600,400});
    r.init();

    auto world = sre::Texture::create()
            .withFile("assets/Arkanos_0.png")
            .withFilterSampling(false)
            .build();
    this->world = SpriteAtlas::createSingleSprite(world,"world", {0,0});

    buildMainMenu();
    buildGame();

    r.frameRender = [&]() {
        render();
    };
    r.keyEvent = [&](SDL_Event &event){
        for (auto & go : currentScene ->getSceneObjects()){
            for (auto & comp : go->getComponents()){
                comp->onKey(event);
            }
        }
    };
    r.frameUpdate = [&](float deltaTime){
        for (auto & go : currentScene ->getSceneObjects()){
            for (auto & comp : go->getComponents()){
                comp->update(deltaTime);
            }
        }
    };

    r.startEventLoop();
};

void RPG::buildMainMenu(){
    auto gameObject = mainMenu.createGameObject();
    auto mmc = gameObject->addComponent<MainMenuComponent>();
    mmc->setRPG(this);

    auto sprite = world->get("world");

    auto spriteComp = gameObject->addComponent<SpriteComponent>();
    spriteComp->setSprite(sprite);

    mainMenu.getCamera().setWindowCoordinates();
}

void RPG::buildGame(){
    auto gameObject = game.createGameObject();

    auto sprite = world->get("world");

    auto spriteComp = gameObject->addComponent<SpriteComponent>();
    spriteComp->setSprite(sprite);

    game.getCamera().setWindowCoordinates();

    auto heroGameObject = game.createGameObject();
    heroGameObject->addComponent<Hero>();
}

void RPG::play(){
    currentScene = &game;
}

void RPG::render(){
    RenderPass rp = RenderPass::create()
            .withCamera(currentScene->getCamera())
            .build();
    // render sprites
    auto sb = SpriteBatch::create();
    for (auto & go : currentScene ->getSceneObjects()){
        for (auto & comp : go->getComponents()){
            comp->renderSprite(sb);
        }
    }
    rp.draw(sb.build());

    // render gui
    for (auto & go : currentScene ->getSceneObjects()){
        for (auto & comp : go->getComponents()){
            comp->onGui();
        }
    }
}

int main(){
    RPG rpg;
}