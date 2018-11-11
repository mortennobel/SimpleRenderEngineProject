//
// Created by Morten Nobel Jørgensen on 2018-11-06.
//


#pragma once

#include <sre/Mesh.hpp>
#include "GameObject.hpp"
#include <sre/Material.hpp>
#include "Transform.hpp"
#include "Renderable.hpp"

class MeshRenderer : public Component, public Renderable {
public:
    explicit MeshRenderer(GameObject* gameObject);
    ~MeshRenderer() override;

    void setMesh(std::shared_ptr<sre::Mesh> mesh);
    std::shared_ptr<sre::Mesh> getMesh();

    void setMaterial(std::shared_ptr<sre::Material> material);
    std::shared_ptr<sre::Material> getMaterial();

    void draw(sre::RenderPass* renderPass) override;

    void debugGUI() override ;

private:
    std::shared_ptr<sre::Mesh> mesh;
    std::shared_ptr<sre::Material>  material;
    Transform* transform;
};
