//
// Created by Morten Nobel Jørgensen on 2018-10-30.
//
#include "Scene.h"

using namespace std;
using namespace sre;

std::shared_ptr<GameObject> Scene::createGameObject() {
    auto obj = shared_ptr<GameObject>(new GameObject());
    sceneObjects.push_back(obj);
    return obj;
}

const std::vector<std::shared_ptr<GameObject>>& Scene::getSceneObjects() {
    return sceneObjects;
}

Camera& Scene::getCamera() {
    return camera;
}