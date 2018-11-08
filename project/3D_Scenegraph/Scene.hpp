//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#pragma once

#include "Renderable.hpp"
#include "Updatable.hpp"
#include "glm/glm.hpp"
#include <vector>

class Camera;
class GameObject;
class Component;
class Light;

class Scene {
public:
    Scene(std::string name = "MainScene");

    void render();
    void update(float deltaTime);

    std::shared_ptr<GameObject> createGameObject(std::string name = "");
private:
    std::string name;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    glm::vec3 ambientColor = {0.5f,0.5f,0.5f};
    std::vector<Camera*> cameras;
    std::vector<Renderable*> renderables;
    std::vector<Updatable*> updatables;
    std::vector<Light*> lights;
    sre::WorldLights worldLights;

    void addComponent(Component* component);
    void removeComponent(Component* component);

    friend class GameObject;
};

