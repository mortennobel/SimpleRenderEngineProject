//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include "BulletPhysics.hpp"

BulletPhysics::BulletPhysics() {
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


/*

btBroadphaseInterface* broadphase = new btDbvtBroadphase();

btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

dynamicsWorld->setGravity(btVector3(0, -10, 0));


btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

btCollisionShape* fallShape = new btSphereShape(1);


btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
btRigidBody::btRigidBodyConstructionInfo
        groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
dynamicsWorld->addRigidBody(groundRigidBody);


btDefaultMotionState* fallMotionState =
        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
btScalar mass = 1;
btVector3 fallInertia(0, 0, 0);
fallShape->calculateLocalInertia(mass, fallInertia);
btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
dynamicsWorld->addRigidBody(fallRigidBody);

for (int i = 0; i < 300; i++) {
dynamicsWorld->stepSimulation(1 / 60.f, 10);

btTransform trans;
fallRigidBody->getMotionState()->getWorldTransform(trans);

std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;
}

dynamicsWorld->removeRigidBody(fallRigidBody);
delete fallRigidBody->getMotionState();
delete fallRigidBody;

dynamicsWorld->removeRigidBody(groundRigidBody);
delete groundRigidBody->getMotionState();
delete groundRigidBody;


delete fallShape;

delete groundShape;

delete dynamicsWorld;
delete solver;
delete collisionConfiguration;
delete dispatcher;
delete broadphase;*/