//
// Created by Morten Nobel Jørgensen on 2018-10-30.
//

#include <SDL_events.h>
#include <sre/Renderer.hpp>
#include "Hero.hpp"
#include "SpriteComponent.hpp"
#include "GameObject.hpp"
#include "sre/SpriteAtlas.hpp"
#include "sre/Texture.hpp"

using namespace sre;

Hero::Hero(GameObject *gameObject)
: Component(gameObject), velocity(0) {
    auto heroTexture = sre::Texture::create()
            .withFile("assets/Goblin.png")
            .withFilterSampling(false)
            .build();

    heroSpriteAtlas = SpriteAtlas::createSingleSprite(heroTexture, "hero");

    auto spriteComponent = gameObject->addComponent<SpriteComponent>();
    spriteComponent->setSprite(heroSpriteAtlas->get("hero"));

    gameObject->setPosition({305,200});

    // setup font
    auto fonts = ImGui::GetIO().Fonts;
    fonts->AddFontDefault();
    auto fontName = "assets/ProggyTiny.ttf";
    int fontSize = 20;
    ProggyTiny =
            fonts->AddFontFromFileTTF(fontName, fontSize);
}

bool Hero::onKey(SDL_Event &event) {
    switch (event.key.keysym.sym){
        case SDLK_w:
            velocity.y = event.type == SDL_KEYDOWN?1:0;
            break;
        case SDLK_s:
            velocity.y = event.type == SDL_KEYDOWN?-1:0;
            break;
        case SDLK_a:
            velocity.x = event.type == SDL_KEYDOWN?-1:0;
            break;
        case SDLK_d:
            velocity.x = event.type == SDL_KEYDOWN?1:0;
            break;
        case SDLK_h:
            message = "Hello world";
            break;
        case SDLK_SPACE:
            message = "";
            break;
    }
}

void Hero::update(float deltaTime) {
    float speed = 100;
    auto newPos = gameObject->getPosition() + velocity*speed*deltaTime;

    gameObject->setPosition(newPos);
}

void Hero::speechBubble(){
    auto r = Renderer::instance;
    auto winsize = r->getWindowSize();

    // display info over hero
    auto flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove;
    bool* open = nullptr;

    auto heroPos = gameObject->getPosition();
    ImVec2 popupSize(200,50);

    ImVec2 pos (heroPos.x/2 - popupSize.x/2, (winsize.y -heroPos.y)/2 + popupSize.y*2);
    auto cond = ImGuiCond_Always;
    ImGui::SetNextWindowPos(pos, cond);
    ImGui::SetNextWindowSize(popupSize, cond);
    ImGui::Begin("", open, flags);
    // Use custom font
    ImGui::PushFont(ProggyTiny);
    ImGui::Text(message.c_str());
    ImGui::PopFont();
    ImGui::End();
}

void Hero::gameInfo() {
    ImVec2 pos = {0,0};
    auto cond = ImGuiCond_Always;
    ImVec2 pivot = {0,0};
    ImGui::SetNextWindowPos(pos, cond, pivot);
    ImVec2 size = {100,50};
    ImGui::SetNextWindowSize(size, cond);
    auto flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove;
    bool* open = nullptr;
    ImGui::Begin("", open, flags);
    ImGui::PushFont(ProggyTiny);
    ImGui::Text("Score");
    ImGui::SameLine();
    ImGui::PopFont();
    ImGui::End();
}

void Hero::onGui() {
    if (message != ""){
        speechBubble();
    }
    gameInfo();


}
