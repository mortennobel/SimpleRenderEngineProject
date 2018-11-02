//
// Created by Morten Nobel Jørgensen on 2018-10-30.
//

#pragma once

#include "Component.hpp"

class Hero : public Component {
public:
    explicit Hero(GameObject *gameObject);

    void onGui() override;

    bool onKey(SDL_Event &event) override;
    void update(float deltaTime) override;
private:
    void guiGameInfo();
    void guiSpeechBubble();
    void guiInventory();

    std::shared_ptr<sre::SpriteAtlas> heroSpriteAtlas;
    static std::map<std::string, std::shared_ptr<sre::Texture>> inventoryTexture;
    glm::vec2 velocity;
    ImFont* ProggyTiny;

    float speachBubbleTimeOut = 10;
    std::string message = "Hi! AWSD to control.";

    glm::ivec2 heroSize;

    // hero stats
    int health = 10;
    int score = 3;
    std::set<std::string> inventorySet;
};
