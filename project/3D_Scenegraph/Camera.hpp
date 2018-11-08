//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#pragma once

#include <sre/Mesh.hpp>
#include <sre/Camera.hpp>
#include "GameObject.hpp"
#include <sre/Material.hpp>
#include "Transform.hpp"
#include "Renderable.hpp"
#include "Ray.hpp"

class Camera : public Component {
public:
    explicit Camera(GameObject * gameObject);
    ~Camera() override = default;

    void bind();

    bool perspective = true;
    float fovY = 45;
    float size;
    float nearPlane = 0.1;
    float farPlane = 100;
    bool clearColorBuffer = true;
    bool clearDepthBuffer = true;

    sre::Color clearColor = sre::Color(0,0,0,1);

    bool debugGui=true;

    void debugGUI() override ;

    Ray screenPointToRay(glm::vec2 point);

    sre::Camera getCamera();
private:
    Transform* transform;
    sre::Camera camera;
    friend class Engine;
    friend class Scene;
};