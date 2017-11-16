#pragma once

#include "sre/SpriteBatch.hpp"

// Forward declaration
class GameObject;
class PhysicsComponent;



class Component {                                           // Abstract class that adds behavior to a GameObject
public:
    explicit Component(GameObject *gameObject);
    virtual ~Component() = default;

    GameObject *getGameObject();

    virtual bool onKey(SDL_Event &event);                   // The function should return true, if the key event is consumed. This prevents other components to receive the event.

    virtual void update(float deltaTime);
    virtual void renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder);

    virtual void onCollisionStart(PhysicsComponent* comp);
    virtual void onCollisionEnd(PhysicsComponent* comp);
protected:
    GameObject *gameObject;

    friend class GameObject;
};

