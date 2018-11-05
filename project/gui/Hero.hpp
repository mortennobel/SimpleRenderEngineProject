//
// Created by Morten Nobel Jørgensen on 2018-10-30.
//

#pragma once

#include "Component.hpp"
#include "CameraComponent.hpp"

class Hero : public Component {
public:
    explicit Hero(GameObject *gameObject);

    void onGui() override;

    bool onKey(SDL_Event &event) override;
    void update(float deltaTime) override;
    void setCamera(std::shared_ptr<CameraComponent> cam);
private:
    void guiGameInfo();
    void guiSpeechBubble();
    void guiInventory();

    std::shared_ptr<sre::Texture> powerbar;
    glm::vec2 powerbarSize;
    std::shared_ptr<sre::Texture> heartIcons[3];
    glm::vec2 heartSize;
    std::shared_ptr<sre::SpriteAtlas> heroSpriteAtlas;
    static std::map<std::string, std::shared_ptr<sre::Texture>> inventoryTexture;
    glm::vec2 velocity;
    ImFont* ProggyTiny;

    float speachBubbleTimeOut = 10;
    std::string message = "Hi! AWSD to control.";

    std::shared_ptr<CameraComponent> cam;

    glm::ivec2 heroSize;

    // hero stats
    int health = 3; // between 0 and 6
    int score = 42;
    float power = 0.7; // between 0.0 and 1.0
    std::set<std::string> inventorySet;
};
