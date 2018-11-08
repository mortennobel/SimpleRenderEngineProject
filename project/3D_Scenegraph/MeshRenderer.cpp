//
// Created by Morten Nobel Jørgensen on 2018-11-06.
//

#include "MeshRenderer.hpp"

#include <sre/Renderer.hpp>
#include <imgui.h>
#include "MeshRenderer.hpp"

using namespace sre;

MeshRenderer::MeshRenderer(GameObject* gameObject)
:Component(gameObject), transform(gameObject->getComponent<Transform>().get())
{
    material = sre::Shader::getStandardBlinnPhong()->createMaterial();
    static auto sharedMeshCube = Mesh::create().withCube().build();
    mesh = sharedMeshCube;
}

MeshRenderer::~MeshRenderer() {

}

void MeshRenderer::setMesh(std::shared_ptr<sre::Mesh> mesh) {
    MeshRenderer::mesh = std::move(mesh);
}

std::shared_ptr<sre::Mesh> MeshRenderer::getMesh(){
    return mesh;
}

std::shared_ptr<sre::Material> MeshRenderer::getMaterial() {
    return material;
}

void MeshRenderer::setMaterial(std::shared_ptr<sre::Material> material) {
    MeshRenderer::material = std::move(material);
}

void MeshRenderer::draw(sre::RenderPass* renderPass) {
    renderPass->draw(mesh, transform->globalTransform(), material);
}

void MeshRenderer::debugGUI() {
    if (ImGui::TreeNode("MeshRenderer")) {
        if (ImGui::TreeNode("Material")) {
            ImGui::LabelText("Material", "%s", material->getName().c_str());
            auto color = material->getColor();
            if (ImGui::DragFloat4("Color", &(color.r))){
                material->setColor(color);
            }
            auto specularity = material->getSpecularity();
            if (ImGui::ColorEdit3("Specularity Color", &(color.r))){
                material->setSpecularity(specularity);
            }
            if (ImGui::DragFloat("Specularity", &(specularity.a))){
                material->setSpecularity(specularity);
            }
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
}


