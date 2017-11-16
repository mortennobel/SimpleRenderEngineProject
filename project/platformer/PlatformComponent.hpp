#pragma once

#include "Component.hpp"
#include <vector>
#include <memory>
#include "glm/glm.hpp"

class GameObject;

class PlatformComponent : public Component {
public:
    explicit PlatformComponent(GameObject *gameObject);

    void init(std::shared_ptr<sre::SpriteAtlas> spriteAtlas, int x, int y, int startSpriteId, int width, bool kinematic);
    void initWall(std::shared_ptr<sre::SpriteAtlas> spriteAtlas, int x, int y, int startSpriteId, int height);

    void moveTo(glm::vec2 tilePos);
    void update(float deltaTime) override;

    glm::vec2 getPosition();
private:
    std::vector<std::shared_ptr<GameObject>> tiles;
    std::shared_ptr<PhysicsComponent> physics;
    bool kinematic;
    glm::vec2 pos;
    int width;
    float physicsScale;
};