//
// Created by Morten Nobel-Jørgensen on 11/6/17.
//

#pragma once

#include "Component.hpp"
#include <vector>
#include "glm/glm.hpp"

class BirdMovementComponent : public Component {
public:
    explicit BirdMovementComponent(GameObject *gameObject);

    void update(float deltaTime) override;

    glm::vec2 computePositionAtTime(float time);

    int getNumberOfSegments();

    const std::vector<glm::vec2>& getPositions();
    void setPositions(std::vector<glm::vec2> positions);
private:
    std::vector<glm::vec2> positions;
    float time = 0;
};

