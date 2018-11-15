//
// Created by Morten Nobel Jørgensen on 2018-11-10.
//

#include "Component.hpp"
#include "btBulletDynamicsCommon.h"
#include "glm/glm.hpp"

class Transform;


class RigidBody : public Component {
public:
    explicit RigidBody(GameObject* gameObject);
    ~RigidBody() final;
    void initRigidBodyWithSphere(float radius, float mass = 1.0);
    void initRigidBodyWithBox(glm::vec3 halfExtend, float mass = 1.0);
    void initRigidBodyWithStaticPlane(glm::vec3 planeNormal, float planeDist);
    void initRigidBody(btRigidBody::btRigidBodyConstructionInfo info);
    btRigidBody* getRigidBody();
private:
    int collisionStartFrameId;
    int collisionStartUpdateFrameId;
    int collisionStartEndFrameId;
    void updateTransformFromPhysicsWorld();
    btRigidBody* rigidBody = nullptr;
    Transform* transform;

    btCollisionShape* shape = nullptr;
    btDefaultMotionState* fallMotionState = nullptr;

    friend class BulletPhysics;
    friend class Scene;
};


