#pragma once

#include "glm/glm.hpp"

class Particle {
public:
    Particle();

    glm::vec3 position;
    glm::vec3 velocity;
    float rotation = 0;
    float angularVelocity = 0;
    float timeOfBirth = 0;
    float normalizedAge = 0; // between 0.0 and 1.0
    bool alive = false;
    int getIndex();
private:
    int index;
    friend class ParticleSystem;
};
