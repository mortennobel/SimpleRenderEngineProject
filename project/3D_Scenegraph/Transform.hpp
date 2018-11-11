//
// Created by Morten Nobel Jørgensen on 2018-11-06.
//
#pragma once
#include <sre/Mesh.hpp>
#include "glm/glm.hpp"
#include <string>
#include <vector>
#include "Component.hpp"

class GameObject;

class Transform : public Component {
public:
    Transform(GameObject* gameObject);

    glm::vec4 color;
    std::string name;

    glm::vec3 position = glm::vec3(0,0,0);

    // euler rotation in degrees (default value 0,0,0)
    glm::vec3 rotation = glm::vec3(0,0,0);
    glm::vec3 scale = glm::vec3(1,1,1);

    glm::mat4 localRotation();

    glm::mat4 localTransform();
    glm::mat4 globalTransform();

    void debugGUI() override;

    Transform *getParent() const;

    void setParent(Transform *parent);
    const std::vector<Transform*> & getChildren();

    void lookAt(glm::vec3 at,glm::vec3 up);
    void lookAt(Transform* at,glm::vec3 up);
private:
    Transform * parent = nullptr;
    sre::Mesh* mesh = nullptr;
    std::vector<Transform*> children;
};


