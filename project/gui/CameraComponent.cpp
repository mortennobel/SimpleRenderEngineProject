//
// Created by Morten Nobel Jørgensen on 2018-11-04.
//

#include <SDL_events.h>
#include "CameraComponent.hpp"
#include "GameObject.hpp"
#include "sre/Renderer.hpp"

CameraComponent::CameraComponent(GameObject *gameObject) : Component(gameObject) {

}

void CameraComponent::update(float deltaTime) {
    using namespace glm;

    // find min and max dist in world space
    auto inst = sre::Renderer::instance;
    auto minWorldPos = clipToWorldspaceCoordinates({-safeArea, -safeArea, 0});
    auto centerWorldPos = clipToWorldspaceCoordinates({0, 0, 0});
    auto maxWorldPos = clipToWorldspaceCoordinates({+safeArea, +safeArea, 0});

    vec3 pos ( target->getPosition(),0);
    vec3 newPos = glm::min(maxWorldPos, glm::max(minWorldPos, pos));
    if (pos != newPos){
        auto diff = newPos-pos;
        centerWorldPos -= diff;
        this->camera->lookAt(centerWorldPos, centerWorldPos + vec3(0,0,-1), vec3(0,1,0));
    }
}

void CameraComponent::onGui() {

}

void CameraComponent::setTarget(std::shared_ptr<GameObject>& target) {
    this->target = target;
}

void CameraComponent::setSize(float size) {
    this->size = size;
    camera->setOrthographicProjection(size, -1, 1);
}

void CameraComponent::setCamera(sre::Camera *camera) {
    this->camera = camera;
}

glm::vec2 CameraComponent::getWindowCoordinates(glm::vec3 worldpos){
    using namespace glm;
    vec4 w(worldpos, 1.0f);
    auto inst = sre::Renderer::instance;
    auto viewport = static_cast<glm::vec2>(inst->getDrawableSize());
    vec4 clipSpace = camera->getProjectionTransform(viewport) * camera->getViewTransform()*w;
    vec4 ndc = clipSpace/clipSpace.w;
    ndc.y *= -1;
    vec4 winCoords = ndc * 0.5f + 0.5f;
    return vec2(winCoords) * vec2(inst->getWindowSize());
}

bool CameraComponent::onKey(SDL_Event &event) {
    if (event.type==SDL_KEYDOWN){
        switch(event.key.keysym.sym){
            case SDLK_1:
                setSize(size+1);
                break;
            case SDLK_2:
                setSize(size-1);
                break;
        }
    }
	return true;
}

glm::vec3 CameraComponent::clipToWorldspaceCoordinates(glm::vec3 clipCoordinates) {
    using namespace glm;
    auto inst = sre::Renderer::instance;
    auto viewport = static_cast<glm::vec2>(inst->getDrawableSize());
    glm::vec4 worldSpace = glm::inverse(camera->getViewTransform())*glm::inverse(camera->getProjectionTransform(viewport)) * vec4(clipCoordinates, 1.0);
    return glm::vec3(worldSpace);
}
