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
class BulletPhysics;
class RigidBody;

class Scene {
public:
    explicit Scene(std::string name = "MainScene");
    ~Scene();
    void render();
    void update(float deltaTime);

    std::shared_ptr<GameObject> createGameObject(std::string name = "Scene");

    bool isDebugPhysics() const;

    void setDebugPhysics(bool debugPhysics);

    const glm::vec3 &getAmbientColor() const;

    void setAmbientColor(const glm::vec3 &ambientColor);

    const std::vector<std::shared_ptr<GameObject>> getGameObjects();
private:
    std::string name;
    bool debugPhysics = true;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    glm::vec3 ambientColor = {0.5f,0.5f,0.5f};
    std::vector<Camera*> cameras;
    std::vector<Renderable*> renderables;
    std::vector<Updatable*> updatables;
    std::vector<RigidBody*> rigidBodies;
    std::vector<Light*> lights;
    sre::WorldLights worldLights;

    void addComponent(Component* component);
    void removeComponent(Component* component);

    BulletPhysics* bulletPhysics;

    friend class GameObject;
    friend class RigidBody;
};

