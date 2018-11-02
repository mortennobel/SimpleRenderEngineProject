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

std::map<std::string, std::shared_ptr<sre::Texture>> Hero::inventoryTexture;

Hero::Hero(GameObject *gameObject)
: Component(gameObject), velocity(0) {
    auto heroTexture = sre::Texture::create()
            .withFile("assets/Goblin.png")
            .withFilterSampling(false)
            .build();

    inventorySet = {"Antidote",
            "Axe",
            "BlueMagic",
            "Carrot",
            "Hat",
            "Lemon",
            "Necklace",
            "Shield"};

    heroSpriteAtlas = SpriteAtlas::createSingleSprite(heroTexture, "hero");

    auto spriteComponent = gameObject->addComponent<SpriteComponent>();
    auto sprite = heroSpriteAtlas->get("hero");
    heroSize = sprite.getSpriteSize();
    spriteComponent->setSprite(sprite);

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
            speachBubbleTimeOut = 5;
            message = "Hello world";
            break;
    }
}

void Hero::update(float deltaTime) {
    float speed = 100;
    auto newPos = gameObject->getPosition() + velocity*speed*deltaTime;

    gameObject->setPosition(newPos);
    speachBubbleTimeOut -= deltaTime;
    if (speachBubbleTimeOut < 0){
        message = "";
    }

}

void Hero::guiSpeechBubble(){
    auto r = Renderer::instance;
    auto winsize = r->getWindowSize();

    // display info over hero
    auto flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar;
    bool* open = nullptr;

    auto heroPos = gameObject->getPosition();
    ImVec2 popupSize(300, 50);

    ImVec2 pos (heroPos.x - popupSize.x / 2, (winsize.y - heroPos.y) - popupSize.y - heroSize.y );
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

void Hero::guiGameInfo() {
    auto r = Renderer::instance;
    auto winsize = r->getWindowSize();
    ImVec2 size = {220,60};
    ImVec2 pos = {winsize.x - size.x,0};
    auto cond = ImGuiCond_Always;
    ImVec2 pivot = {0,0};
    ImGui::SetNextWindowPos(pos, cond, pivot);

    ImGui::SetNextWindowSize(size, cond);
    auto flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar;
    bool* open = nullptr;
    ImGui::Begin("#gameinfo", open, flags);
    ImGui::PushFont(ProggyTiny);

    // draw score
    ImGui::Text("Score"); ImGui::SameLine();
    auto scoreStr = std::to_string(score);
    float windowWidth =    ImGui::GetWindowContentRegionWidth();
    float width = ImGui::CalcTextSize(scoreStr.c_str()).x;
    ImGui::SetCursorPosX(windowWidth - width); // align right
    ImGui::Text(scoreStr.c_str());

    // draw health
    ImGui::PushID(1);
    auto healthStr = std::to_string(health);
    ImGui::Text("Health"); ImGui::SameLine();
    width = ImGui::CalcTextSize(healthStr.c_str()).x;
    ImGui::SetCursorPosX(windowWidth - width); // align right
    ImGui::Text(healthStr.c_str());
    ImGui::PopID();
    ImGui::PopFont();
    ImGui::End();
}

void Hero::guiInventory() {
    ImVec2 pos = {0,0};
    auto cond = ImGuiCond_Always;
    ImVec2 pivot = {0,0};
    ImGui::SetNextWindowPos(pos, cond, pivot);
    ImVec2 size = {165, 107};
    ImGui::SetNextWindowSize(size, cond);
    auto flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar;
    bool* open = nullptr;
    ImGui::Begin("#inventory", open, flags);
    ImGui::PushFont(ProggyTiny);
    ImGui::Text("Inventory");

    int count = 0;
    for (auto& item : inventorySet){
        auto hasItem = inventoryTexture.find(item) != inventoryTexture.end();
        if (!hasItem){
            auto filename = std::string("assets/")+item+".png";
            inventoryTexture[item] = Texture::create().withFile(filename).withFilterSampling(false).build();
        }
        ImVec2 uv0(0,1); // flip y axis coordinates
        ImVec2 uv1(1,0);
        ImVec2 s(30,30);
        ImGui::Image(inventoryTexture[item]->getNativeTexturePtr(), s, uv0, uv1 , ImVec4(1,1,1,1),ImVec4(0,0,0,1));

        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(item.c_str());
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }

        if (count == 0 || count %4 != 0)
            ImGui::SameLine();
        count ++;
    }

    ImGui::PopFont();
    ImGui::End();
}

void Hero::onGui() {
    if (!message.empty()){
        guiSpeechBubble();
    }
    guiGameInfo();
    guiInventory();
}
