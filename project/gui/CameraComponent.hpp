//
// Created by Morten Nobel Jørgensen on 2018-11-04.
//
#pragma once

#include "Component.hpp"
#include "sre/Camera.hpp"

// Create a game camera
class CameraComponent : public Component{
public:
    explicit CameraComponent(GameObject *gameObject);

    void update(float deltaTime) override;

    void onGui() override;

    void setCamera(sre::Camera* camera);

    void setSize(float size);

    bool onKey(SDL_Event &event) override;

    void setTarget(std::shared_ptr<GameObject>& target);

    glm::vec2 getWindowCoordinates(glm::vec3 worldpos);
    glm::vec3 clipToWorldspaceCoordinates(glm::vec3 clipCoordinates);
private:
    sre::Camera* camera;
    float size = 400;
    std::shared_ptr<GameObject> target;
    float safeArea = 0.3f; // percentage from center
};

