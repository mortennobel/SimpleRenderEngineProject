//
// Created by Morten Nobel Jørgensen on 2018-11-15.
//

#pragma once

class CollisionHandler {
public:
    virtual void onCollision(size_t collisionId, RigidBody* other, glm::vec3 position, bool begin) = 0; // begin or update
    virtual void onCollisionEnd(size_t collisionId) = 0;
};