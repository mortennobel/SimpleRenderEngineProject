//
// Created by Morten Nobel-Jørgensen on 11/6/17.
//

#include "MovingPlatformComponent.hpp"
#include "PlatformComponent.hpp"
#include "GameObject.hpp"

MovingPlatformComponent::MovingPlatformComponent(GameObject *gameObject) : Component(gameObject)
{
    platformComponent = gameObject->getComponent<PlatformComponent>();
}

void MovingPlatformComponent::update(float deltaTime) {
    totalTime += deltaTime;

    // todo replace with easing function
    if (fmod(totalTime,2)>1){
        platformComponent->moveTo(movementEnd);
    } else {
        platformComponent->moveTo(movementStart);
    }
}

void MovingPlatformComponent::setMovementStart(glm::vec2 pos) {
    movementStart = pos;
}

void MovingPlatformComponent::setMovementEnd(glm::vec2 pos) {
    movementEnd = pos;
}
