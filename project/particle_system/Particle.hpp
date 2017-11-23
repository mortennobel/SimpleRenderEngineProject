#pragma once

#include "glm/glm.hpp"

class Particle {
public:
    Particle(glm::vec3& position, glm::vec4& color, float& size, glm::vec4& uv);

    // physics
    glm::vec3& position;
    glm::vec3 velocity;
    float rotation = 0;
    float angularVelocity = 0;

    // lifetime
    float timeOfBirth = 0;
    float normalizedAge = 0; // between 0.0 and 1.0
    bool alive = false;

    // appearance
    glm::vec4& color;
    float& size;
    glm::vec4& uv;

    int getIndex();
private:
    int index;
    friend class ParticleSystem;
};
