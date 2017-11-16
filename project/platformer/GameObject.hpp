#include <memory>
#include <vector>
#include "glm/glm.hpp"
#include "sre/SpriteBatch.hpp"
#pragma once

// Forward declaration
class Component;

// GameObject are empty container objects, which contains Components
class GameObject {
public:

    ~GameObject();

    template <class T>                                                  // Add component of a given type to a gameObject. example:
    std::shared_ptr<T> addComponent();                   // std::shared_ptr<SpriteComponent> spriteComponent = gameObject->addComponent<SpriteComponent>();

    template <class T>                                   //  Get component of a given type to a gameObject. If not found return empty shared_ptr (==nullptr). example:
    std::shared_ptr<T> getComponent();                   // std::shared_ptr<SpriteComponent> spriteComponent = gameObject->getComponent<SpriteComponent>();

    bool removeComponent(std::shared_ptr<Component> component);

    void renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder);
    void update(float deltaTime);

    const glm::vec2 &getPosition() const;

    void setPosition(const glm::vec2 &position);

    float getRotation() const;

    void setRotation(float rotation);

    const std::vector<std::shared_ptr<Component>>& getComponents();

    std::string name = "_";
private:
    GameObject() = default;
    std::vector<std::shared_ptr<Component>> components;

    glm::vec2 position;
    float rotation;

    friend class PlatformerGame;
};

// definition of the template member function addComponent
// Usage:
// GameObject* go = something;
// shared_ptr<SpriteComponent> sc = go->addComponent<SpriteComponent>();
template <class T>
inline std::shared_ptr<T> GameObject::addComponent(){
    auto obj = std::shared_ptr<T>(new T(this));
    components.push_back(obj);

    return obj;
}

// definition of the template member function addComponent
// Usage:
// GameObject* go = something;
// shared_ptr<SpriteComponent> sc = go->addComponent<SpriteComponent>();
template <class T>
inline std::shared_ptr<T> GameObject::getComponent(){
    for (auto c : components){
        std::shared_ptr<T> res = std::dynamic_pointer_cast<T>(c);
        if (res != nullptr){
            return res;
        }
    }
    return std::shared_ptr<T>();
}
