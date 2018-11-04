//
// Created by Morten Nobel Jørgensen on 2018-10-30.
//

#include "MainMenuComponent.h"
#include "RPG.h"
#include "sre/Renderer.hpp"

using namespace sre;

MainMenuComponent::MainMenuComponent(GameObject *gameObject)
:Component(gameObject)
{
}

void MainMenuComponent::setRPG(RPG* rpg){
    this->rpg = rpg;
}

void MainMenuComponent::playPressed(){
    rpg->play();
}

void MainMenuComponent::settingsPressed(){
    //
}

void MainMenuComponent::onGui() {
    auto r = Renderer::instance;

    auto flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove;
    bool* open = nullptr;

    ImVec2 imageButtonSize(200,50);
    auto winsize = r->getWindowSize();
    ImVec2 iWinSize(224,150);

    ImVec2 pos ((winsize.x - iWinSize.x)/2.0f, winsize.y/2.0f);
    auto cond = ImGuiCond_Always;
    ImGui::SetNextWindowPos(pos, cond);
    ImGui::SetNextWindowSize(iWinSize, cond);
    ImGui::Begin("", open, flags);

    // load textures
    static auto playTex = Texture::create()
            .withFile("assets/play.png")
            .withFilterSampling(false)
            .build();
    static auto settingsTex = Texture::create()
            .withFile("assets/settings.png")
            .withFilterSampling(false)
            .build();

    void* playId = playTex->getNativeTexturePtr();
    void* settingsId = settingsTex->getNativeTexturePtr();

    ImVec2 uv0(0,1); // flip y axis coordinates
    ImVec2 uv1(1,0);

    // Render buttons
    if (ImGui::ImageButton(playId, imageButtonSize, uv0, uv1)){
        playPressed();
    }
    if (ImGui::ImageButton(settingsId, imageButtonSize, uv0, uv1)){
        settingsPressed();
    }
    ImGui::End();
}