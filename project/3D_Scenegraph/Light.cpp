//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include "Light.hpp"

Light::Light(GameObject* gameObject)
:Component(gameObject), light(light.create().withDirectionalLight({0,-1,0}).build()),
 transform(gameObject->getComponent<Transform>().get())
{
}



void Light::debugGUI() {
    if (ImGui::TreeNode("Light")) {
        int lt = (int)light.lightType;
        ImGui::RadioButton("Point", &lt, 0); ImGui::SameLine();
        ImGui::RadioButton("Directional", &lt, 1);
        light.lightType = (sre::LightType)lt;

        ImGui::DragFloat3("Color", &(light.color.x));
        if (light.lightType == sre::LightType::Point){
            ImGui::DragFloat("Range", &(light.range));
        }
        ImGui::TreePop();
    }
}

sre::Light &Light::getLight() {
    if (light.lightType == sre::LightType::Directional){
        auto res = transform->globalTransform() * glm::vec4(0,0,-1,0);
        light.direction = glm::vec3{res.x,res.y,res.z};
    } else {
        auto res = transform->globalTransform() * glm::vec4(0,0,0,1);
        light.position = glm::vec3{res.x,res.y,res.z};
    }
    return light;
}
