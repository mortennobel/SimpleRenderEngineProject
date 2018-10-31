#pragma once
#include "sre/SDLRenderer.hpp"
#include "GameObject.hpp"
#include "Scene.h"

class RPG {
public:
    RPG();
    void play();
private:
    void buildMainMenu();
    void buildGame();
    Scene mainMenu;
    Scene game;
    Scene* currentScene;
    std::shared_ptr<sre::SpriteAtlas> world;
    sre::SDLRenderer r;
    void render();
};

