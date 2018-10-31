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
    void gameInfo();
    void speechBubble();
    std::shared_ptr<sre::SpriteAtlas> heroSpriteAtlas;
    glm::vec2 velocity;
    ImFont* ProggyTiny;
    std::string message = "Hi!";

    // hero stats
    int health;
    int score;
};
