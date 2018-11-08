#include <iostream>
#include "sre/SDLRenderer.hpp"
#include "Scene.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "MeshRenderer.hpp"
#include "Light.hpp"


std::shared_ptr<Scene> createScene(){
    auto res = std::make_shared<Scene>();
    auto cameraObj = res->createGameObject("Camera");
    cameraObj->addComponent<Camera>();
    cameraObj->getComponent<Transform>()->position = {0,1.7f,10};

    auto sphereObj = res->createGameObject("Sphere");
    auto sphereMR = sphereObj->addComponent<MeshRenderer>();
    sphereMR->setMesh(sre::Mesh::create().withSphere().build());
    sphereObj->getComponent<Transform>()->position = {0,1.0,0};

    auto planeObj = res->createGameObject("Plane");
    auto plameMR = planeObj->addComponent<MeshRenderer>();
    plameMR->setMesh(sre::Mesh::create().withQuad(10).build());
    planeObj->getComponent<Transform>()->rotation = {-90,0,0};

    auto lightObj = res->createGameObject("Light");
    lightObj->addComponent<Light>();

    return res;
}

int main(){
    using namespace sre;
    SDLRenderer r;
    r.init();
    auto scene = createScene();

    r.frameUpdate = [&](float deltaTime){
        scene->update(deltaTime);
    };
    r.frameRender = [&]{
        scene->render();
    };

    r.startEventLoop();

    return 0;
}
