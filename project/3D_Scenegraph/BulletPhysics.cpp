//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include "BulletPhysics.hpp"
#include "RigidBody.hpp"
#include "GameObject.hpp"

bool contactUpdatedCallback(btManifoldPoint& cp,void* body0,void* body1){
    bool collisionBegin = cp.m_userPersistentData == nullptr;
    auto btRigidBody0 = static_cast<btRigidBody *>(body0);
    auto btRigidBody1 = static_cast<btRigidBody *>(body1);
    auto rigidBody0 = static_cast<RigidBody *>(btRigidBody0->getUserPointer());
    auto rigidBody1 = static_cast<RigidBody *>(btRigidBody1->getUserPointer());
    if (collisionBegin){
        static size_t collisionId = 0;

        cp.m_userPersistentData = (void*)collisionId++;
    }
    auto id = (size_t)cp.m_userPersistentData;
    glm::vec3 pointOnA (cp.getPositionWorldOnA().x(), cp.getPositionWorldOnA().y(), cp.getPositionWorldOnA().z());
    glm::vec3 pointOnB (cp.getPositionWorldOnB().x(), cp.getPositionWorldOnB().y(), cp.getPositionWorldOnB().z());
    for (auto ph : rigidBody0->getGameObject()->getCollisionHandlers()){
        ph->onCollision(id, rigidBody1, pointOnA, collisionBegin);
    }
    for (auto ph : rigidBody1->getGameObject()->getCollisionHandlers()){
        ph->onCollision(id, rigidBody0, pointOnB, collisionBegin);
    }
    return true;
}

bool contactDestroyedCallback(void * data) {
    // todo
    return true;
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

void BulletPhysics::step() {
    world->stepSimulation(timeStep, maxSubSteps);

}

void BulletPhysics::debugDrawNewFrame() {
    debugDrawObj.newFrame();
    world->debugDrawWorld();
}

void BulletPhysics::debugDraw(sre::RenderPass &renderPass) {
    debugDrawObj.render(renderPass);
}
