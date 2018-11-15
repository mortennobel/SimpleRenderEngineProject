//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//
#pragma once

#include "btBulletDynamicsCommon.h"
#include "glm/glm.hpp"
#include "BulletDebugDrawSRE.hpp"
#include "sre/RenderPass.hpp"

class Scene;

class BulletPhysics {
public:
    BulletPhysics();
    ~BulletPhysics();

    const glm::vec3 &getGravity() const;
    void setGravity(const glm::vec3 &gravity);

    void step(Scene* scene);

    void debugDrawNewFrame();
    void debugDraw(sre::RenderPass& renderPass);
private:
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* world;

    glm::vec3 gravity = {0, -9.8f, 0};
    float timeStep = 1/60.0f;
    int maxSubSteps = 10;

    BulletDebugDrawSRE debugDrawObj;

    friend class RigidBody;
};
