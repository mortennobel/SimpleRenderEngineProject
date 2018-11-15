//
// Created by Morten Nobel Jørgensen on 2018-11-06.
//

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Scene.hpp"
#include "CollisionHandler.hpp"

class Component;


class GameObject {
public:
    ~GameObject();

    template<typename C>
    std::shared_ptr<C> addComponent();

    bool removeComponent(std::shared_ptr<Component> ptr);

    template<typename C>
    std::shared_ptr<C> getComponent();

    std::string getName();
    void setName(const std::string &name);

    const std::vector<std::shared_ptr<Component>>& getComponents();
    Scene* getScene();

    const std::vector<CollisionHandler*>& getCollisionHandlers();
private:
    Scene* scene;
    std::vector<std::shared_ptr<Component>> components;
    std::vector<CollisionHandler*> collisionHandlers;
    GameObject(std::string name, Scene* scene);
    std::string name;
    friend class Scene;
};

// function templates has to defined in header files
template<typename T>
std::shared_ptr<T> GameObject::addComponent() {
    auto obj = new T(this);
    auto ch = dynamic_cast<CollisionHandler*>(obj);
    if (ch){
        collisionHandlers.push_back(ch);
    }
    scene->addComponent(obj);
    std::shared_ptr<T> ptr(obj);
    components.push_back(ptr);
    return ptr;
}

template<typename C>
std::shared_ptr<C> GameObject::getComponent() {
    for (auto c : components){
        auto castPtr = std::dynamic_pointer_cast<C>(c);
        if (castPtr != nullptr){
            return castPtr;
        }
    }
    // not found
    return nullptr;
}