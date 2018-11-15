//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include "BulletPhysics.hpp"
#include "RigidBody.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"

struct CollisionId {
    size_t collisionId;
    GameObject* bodyA;
    GameObject* bodyB;

    CollisionId(size_t collisionId, GameObject *bodyA, GameObject *bodyB) : collisionId(collisionId), bodyA(bodyA),
                                                                            bodyB(bodyB) {}
};

namespace {
 Scene* scene;

 bool sceneContains(GameObject* ptr){
     for (auto go : scene->getGameObjects()){
         if (go.get() == ptr){
             return true;
         }
     }
     return false;
 }

// based on https://github.com/kripken/ammo.js/issues/82#issuecomment-426429129
bool contactUpdatedCallback(btManifoldPoint& cp,void* body0,void* body1){
    bool collisionBegin = cp.m_userPersistentData == nullptr;
    auto btRigidBody0 = static_cast<btRigidBody *>(body0);
    auto btRigidBody1 = static_cast<btRigidBody *>(body1);
    auto rigidBody0 = static_cast<RigidBody *>(btRigidBody0->getUserPointer());
    auto rigidBody1 = static_cast<RigidBody *>(btRigidBody1->getUserPointer());
    if (collisionBegin){
        static size_t collisionId = 0;
        collisionId++;
        cp.m_userPersistentData = new CollisionId(collisionId, rigidBody0->getGameObject(), rigidBody1->getGameObject());
    }
    CollisionId* id = (CollisionId*)cp.m_userPersistentData;
    glm::vec3 pointOnA (cp.getPositionWorldOnA().x(), cp.getPositionWorldOnA().y(), cp.getPositionWorldOnA().z());
    glm::vec3 pointOnB (cp.getPositionWorldOnB().x(), cp.getPositionWorldOnB().y(), cp.getPositionWorldOnB().z());
    for (auto ph : rigidBody0->getGameObject()->getCollisionHandlers()){
        ph->onCollision(id->collisionId, rigidBody1, pointOnA, collisionBegin);
    }
    for (auto ph : rigidBody1->getGameObject()->getCollisionHandlers()){
        ph->onCollision(id->collisionId, rigidBody0, pointOnB, collisionBegin);
    }
    return true;
}

bool contactDestroyedCallback(void * data) {
    CollisionId* id = (CollisionId*)data;
    if (sceneContains(id->bodyA)){
        for (auto ph : id->bodyA->getCollisionHandlers()){
            ph->onCollisionEnd(id->collisionId);
        }
    }
    if (sceneContains(id->bodyB)){
        for (auto ph : id->bodyB->getCollisionHandlers()){
            ph->onCollisionEnd(id->collisionId);
        }
    }
    delete id;
    return true;
}
}

BulletPhysics::BulletPhysics() {
    gContactProcessedCallback = contactUpdatedCallback;
    gContactDestroyedCallback = contactDestroyedCallback;
    // gContactDestroyedCallback = contactDestroyedCallback;
    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver();
    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    world->setDebugDrawer(&debugDrawObj);
    setGravity(gravity);
}

BulletPhysics::~BulletPhysics() {
    delete world;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;
}

const glm::vec3 &BulletPhysics::getGravity() const {
    return gravity;
}

void BulletPhysics::setGravity(const glm::vec3 &gravity) {
    BulletPhysics::gravity = gravity;
    world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

void BulletPhysics::step(Scene* scene_) {
    scene = scene_;
    world->stepSimulation(timeStep, maxSubSteps);

}

void BulletPhysics::debugDrawNewFrame() {
    debugDrawObj.newFrame();
    world->debugDrawWorld();
}

void BulletPhysics::debugDraw(sre::RenderPass &renderPass) {
    debugDrawObj.render(renderPass);
}
