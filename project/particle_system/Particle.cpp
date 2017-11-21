//
// Created by Morten Nobel-Jørgensen on 11/21/17.
//

#include "Particle.hpp"
#include <limits>

Particle::Particle()
:timeOfBirth(-std::numeric_limits<float>::max()),
 alive(false)
{
}

int Particle::getIndex() {
    return index;
}
