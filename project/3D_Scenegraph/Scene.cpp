//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include "Scene.hpp"
#include <algorithm>
#include <sre/Renderer.hpp>
#include "Camera.hpp"
#include "GameObject.hpp"
#include "RigidBody.hpp"
#include "Light.hpp"
#include "BulletPhysics.hpp"
#include "sre/RenderPass.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"


Scene::Scene(std::string name)
:name(std::move(name))
{
    bulletPhysics = new BulletPhysics();
}

Scene::~Scene(){
    delete bulletPhysics;
	bulletPhysics = nullptr;
}

void Scene::update(float deltaTime){
    bulletPhysics->step(this);
    for (auto& p : this->rigidBodies){
        p->updateTransformFromPhysicsWorld();
    }
    for (auto& u : updatables){
        u->update(deltaTime);
    }
}

void Scene::render(){
    if (debugPhysics){
        bulletPhysics->debugDrawNewFrame();
    }
    worldLights.clear();
    worldLights.setAmbientLight(ambientColor);
    if (lights.size() > sre::Renderer::instance->getMaxSceneLights()){
        // instead for each rendered object closest lights and update wl
        std::cout << "Warn - too many scene lights" << std::endl;
    }
    for (auto & l : lights){
        worldLights.addLight(l->getLight());
    }

    for (auto c : cameras){
        c->bind();

        auto rp = sre::RenderPass::create()
                .withCamera(c->getCamera())
                .withClearColor(c->clearColorBuffer, c->clearColor)
                .withClearDepth(c->clearDepthBuffer)
                .withWorldLights(&worldLights)
                .withGUI(c->debugGui)
                .build();

        for (auto& comp : renderables){
            comp->draw(&rp);
        }
        if (debugPhysics){
            bulletPhysics->debugDraw(rp);
        }
        if (c->debugGui){
            ImGui::Begin(name.c_str());
            ImGui::Checkbox("Debug Physics", &debugPhysics);
            ImGui::ColorEdit3("Ambient light", &(ambientColor.x));
            if (ImGui::CollapsingHeader("GameObjects")){
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
            ImGui::End();
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
    auto rigidBody = dynamic_cast<RigidBody*>(component);
    if (rigidBody) {
        rigidBodies.push_back(rigidBody);
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
    auto rigidBody = dynamic_cast<RigidBody*>(component);
    if (rigidBody) {
        rigidBodies.erase(std::find(rigidBodies.begin(), rigidBodies.end(), rigidBody));
    }
}

bool Scene::isDebugPhysics() const {
    return debugPhysics;
}

void Scene::setDebugPhysics(bool debugPhysics) {
    Scene::debugPhysics = debugPhysics;
}

const glm::vec3 &Scene::getAmbientColor() const {
    return ambientColor;
}

void Scene::setAmbientColor(const glm::vec3 &ambientColor) {
    Scene::ambientColor = ambientColor;
}

const std::vector<std::shared_ptr<GameObject>> Scene::getGameObjects() {
    return gameObjects;
}


#pragma clang diagnostic pop