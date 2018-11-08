//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include "Scene.hpp"
#include <algorithm>
#include "Camera.hpp"
#include "GameObject.hpp"
#include "Light.hpp"
#include "sre/RenderPass.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"


Scene::Scene(std::string name)
:name(name)
{
}

void Scene::update(float deltaTime){
    for (auto& u : updatables){
        u->update(deltaTime);
    }
}

void Scene::render(){
    sre::WorldLights wl;
    wl.setAmbientLight(ambientColor);
    for (auto & l : lights){
        wl.addLight(l->getLight());
    }

    for (auto c : cameras){
        c->bind();

        auto rp = sre::RenderPass::create()
                .withCamera(c->getCamera())
                .withClearColor(c->clearColorBuffer, c->clearColor)
                .withClearDepth(c->clearDepthBuffer)
                .withWorldLights(&wl)
                .withGUI(c->debugGui)
                .build();

        for (auto& comp : renderables){
            comp->draw(&rp);
        }
        if (c->debugGui){
            for (auto& go : gameObjects){
                ImGui::PushID(go.get());
                if (ImGui::CollapsingHeader(go->getName().c_str())){
                    for (auto& co : go->getComponents()) {
                        co->debugGUI();
                    }
                }
                ImGui::PopID();
            }
        }
    }
}

std::shared_ptr<GameObject> Scene::createGameObject(std::string name){
    auto res = std::shared_ptr<GameObject>(new GameObject(name, this));
    gameObjects.push_back(res);
    return res;
}

void Scene::addComponent(Component *component) {
    auto camera = dynamic_cast<Camera*>(component);
    if (camera){
        cameras.push_back(camera);
    }
    auto renderable = dynamic_cast<Renderable*>(component);
    if (renderable) {
        renderables.push_back(renderable);
    }
    auto updatable = dynamic_cast<Updatable*>(component);
    if (updatable) {
        updatables.push_back(updatable);
    }
    auto light = dynamic_cast<Light*>(component);
    if (light) {
        lights.push_back(light);
    }
}

void Scene::removeComponent(Component *component) {
    auto camera = dynamic_cast<Camera*>(component);
    if (camera){
        cameras.erase(std::find(cameras.begin(),cameras.end(), camera));
    }
    auto renderable = dynamic_cast<Renderable*>(component);
    if (renderable) {
        renderables.erase(std::find(renderables.begin(), renderables.end(), renderable));
    }
    auto updatable = dynamic_cast<Updatable*>(component);
    if (updatable) {
        updatables.erase(std::find(updatables.begin(), updatables.end(), updatable));
    }
    auto light = dynamic_cast<Light*>(component);
    if (light) {
        lights.erase(std::find(lights.begin(), lights.end(), light));
    }
}


#pragma clang diagnostic pop