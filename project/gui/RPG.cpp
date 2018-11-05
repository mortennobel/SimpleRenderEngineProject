//
// Created by Morten Nobel Jørgensen on 2018-10-29.
//

#include "RPG.h"
#include "Component.hpp"
#include "Hero.hpp"
#include "MainMenuComponent.h"
#include "SpriteComponent.hpp"
#include "CameraComponent.hpp"
#include "sre/SpriteAtlas.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

using namespace sre;

const glm::vec2 heroStartingPos(305,200);

RPG::RPG()
:currentScene(&mainMenu)
{
    r.setWindowSize({600,450});
    r.init()
     .withSdlWindowFlags(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)
     .build();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    auto inst = Renderer::instance;
    std::cout << "getWindowSize "<<glm::to_string(inst->getWindowSize()) << std::endl;
    std::cout << "getDrawableSize "<<glm::to_string(inst->getDrawableSize()) << std::endl;

    auto world = sre::Texture::create()
            .withFile("assets/Arkanos_0.png")
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

    mainMenu.getCamera().setOrthographicProjection(200, -1, 1);
    mainMenu.getCamera().lookAt({heroStartingPos,0},{heroStartingPos,-1},{0,1,0});
}

void RPG::buildGame(){
    auto gameObject = game.createGameObject();

    auto sprite = world->get("world");

    auto spriteComp = gameObject->addComponent<SpriteComponent>();
    spriteComp->setSprite(sprite);

    auto heroGameObject = game.createGameObject();

    auto camereObj = game.createGameObject();
    auto cameraComp = camereObj->addComponent<CameraComponent>();
    cameraComp->setCamera(&game.getCamera());
    game.getCamera().lookAt({heroStartingPos,0},{heroStartingPos,-1},{0,1,0});
    cameraComp->setSize(200);
    cameraComp->setTarget(heroGameObject);

    auto h = heroGameObject->addComponent<Hero>();
    heroGameObject->setPosition({heroStartingPos});
    h->setCamera(cameraComp);
}

void RPG::play(){
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags ^= ImGuiConfigFlags_NavEnableKeyboard;  // Disable Keyboard Controls
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
    for (auto & go : currentScene->getSceneObjects()){
        for (auto & comp : go->getComponents()){
            comp->onGui();
        }
    }
}

int main(){
    RPG rpg;
}