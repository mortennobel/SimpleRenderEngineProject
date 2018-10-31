//
// Created by Morten Nobel Jørgensen on 2018-10-30.
//
#include "sre/Camera.hpp"
#include "GameObject.hpp"

class Scene {
public:
    std::shared_ptr<GameObject> createGameObject();
    sre::Camera& getCamera();
    const std::vector<std::shared_ptr<GameObject>>& getSceneObjects();
private:
    std::vector<std::shared_ptr<GameObject>> sceneObjects;
    sre::Camera camera;
};


