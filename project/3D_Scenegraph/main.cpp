#include <iostream>
#include "sre/SDLRenderer.hpp"
#include "Scene.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "MeshRenderer.hpp"
#include "Light.hpp"
#include "RigidBody.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

class CustomCollisionHandler : public Component, public CollisionHandler {
public:
    explicit CustomCollisionHandler(GameObject *gameObject) : Component(gameObject) {}

    void onCollision(size_t collisionId, RigidBody *other, glm::vec3 position, bool begin) override {
        if (begin){
            std::cout << "Collision "<< collisionId <<" on "<<other->getGameObject()->getName()<< " at "<<glm::to_string(position)<<std::endl;
        }
    }
    void onCollisionEnd(size_t collisionId) override {
        std::cout << "Collision end "<<collisionId<<std::endl;
    }
};

std::shared_ptr<Scene> createScene(){
    auto res = std::make_shared<Scene>();
    auto cameraObj = res->createGameObject("Camera");
    cameraObj->addComponent<Camera>()->clearColor = {0.2,0.2,0.2};
    cameraObj->getComponent<Transform>()->position = {0,1.7f,10};

    auto sphereObj = res->createGameObject("Sphere");
    auto sphereMR = sphereObj->addComponent<MeshRenderer>();
    sphereMR->setMesh(sre::Mesh::create().withSphere(16,32,0.99f).build());
    sphereObj->getComponent<Transform>()->position = {0,1.0,0};
    sphereObj->getComponent<Transform>()->position = {0,1.0,0};
    sphereObj->addComponent<RigidBody>()->initRigidBodyWithSphere(1, 0);

    auto planeObj = res->createGameObject("Plane");
    auto plameMR = planeObj->addComponent<MeshRenderer>();
    plameMR->setMesh(sre::Mesh::create().withQuad(10).build());
    planeObj->getComponent<Transform>()->rotation = {-90,0,0};
    auto planePhysObj = res->createGameObject("PlanePhys");
    planePhysObj->addComponent<RigidBody>()->initRigidBodyWithStaticPlane({0,1,0}, 0);

    auto lightObj = res->createGameObject("Light");
    lightObj->getComponent<Transform>()->rotation = {30,30,0};
    lightObj->addComponent<Light>();

    auto cube = res->createGameObject("Cube");
    cube->getComponent<Transform>()->position = {0,4,0};
    cube->getComponent<Transform>()->rotation = {30,30,0};
    auto cubeRigidBody = cube->addComponent<RigidBody>();
    cubeRigidBody->initRigidBodyWithBox({1,1,1}, 1);
    auto cubeMR = cube->addComponent<MeshRenderer>();
    cubeMR->setMesh(sre::Mesh::create().withCube(0.99).build());
    cube->addComponent<CustomCollisionHandler>();

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
