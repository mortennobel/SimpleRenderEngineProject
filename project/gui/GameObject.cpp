#include "GameObject.hpp"
#include <cassert>
#include <algorithm>
#include "Component.hpp"

GameObject::~GameObject(){
    // remove reference to this in components
    for (auto& c : components){
        c->gameObject = nullptr;
    }
}

bool GameObject::removeComponent(std::shared_ptr<Component> component) {
    auto comp = std::find(components.begin(), components.end(),component);
    if (comp != components.end()){
        components.erase(comp);
    }
    return false;
}

const glm::vec2 &GameObject::getPosition() const {
    return position;
}

void GameObject::setPosition(const glm::vec2 &position) {
    GameObject::position = position;
}

float GameObject::getRotation() const {
    return rotation;
}

void GameObject::setRotation(float rotation) {
    GameObject::rotation = rotation;
}

void GameObject::renderSprite(sre::SpriteBatch::SpriteBatchBuilder &spriteBatchBuilder) {
    for (auto& comp : components){
        comp->renderSprite(spriteBatchBuilder);
    }
}

void GameObject::update(float deltaTime) {
    for (auto& comp : components){
        comp->update(deltaTime);
    }
}

const std::vector<std::shared_ptr<Component>> &GameObject::getComponents() {
    return components;
}
