//
// Created by Morten Nobel-Jørgensen on 11/21/17.
//

#include "Particle.hpp"
#include <limits>

Particle::Particle(glm::vec3& position, glm::vec4 &color, float &size, glm::vec4 &uv)
        :position(position), timeOfBirth(-std::numeric_limits<float>::max()),
         alive(false), color(color), uv(uv), size(size)
{
}

int Particle::getIndex() {
    return index;
}
