//
// Created by Morten Nobel Jørgensen on 2018-11-06.
//

#include "Component.hpp"

Component::Component(GameObject* gameObject)
:gameObject(gameObject)
{
}

GameObject *Component::getGameObject() {
    return gameObject;
}

void Component::debugGUI() {

}

void Component::onStart() {

}

void Component::onDestroy(){

}
