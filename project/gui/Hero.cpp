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

const int heartEmpty = 0;
const int heartHalf = 1;
const int heartFull = 2;

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

    heartIcons[heartEmpty] = Texture::create().withFile("assets/hud_heartEmpty.png").withFilterSampling(false).build();
    heartIcons[heartHalf] = Texture::create().withFile("assets/hud_heartHalf.png").withFilterSampling(false).build();
    heartIcons[heartFull] = Texture::create().withFile("assets/hud_heartFull.png").withFilterSampling(false).build();
    heartSize = {heartIcons[heartFull]->getWidth()*0.5f, heartIcons[heartFull]->getHeight()*0.5f};
    powerbar = Texture::create().withFile("assets/powerbar.png").withFilterSampling(false).build();
    powerbarSize = {heartSize.x*3,heartSize.y};

    heroSpriteAtlas = SpriteAtlas::createSingleSprite(heroTexture, "hero");

    auto spriteComponent = gameObject->addComponent<SpriteComponent>();
    auto sprite = heroSpriteAtlas->get("hero");
    heroSize = sprite.getSpriteSize();
    spriteComponent->setSprite(sprite);


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
	return true;
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
    auto heroPosWin = cam->getWindowCoordinates(glm::vec3(heroPos, 0.0));
    ImVec2 popupSize(300, 50);

    ImVec2 pos (heroPosWin.x - popupSize.x / 2, heroPosWin.y - popupSize.y - heroSize.y );
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
    ImVec2 size = {180, 107};
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

    // draw health
    ImGui::Text("Health");
    float width = heartSize.x*3;
    float windowWidth = ImGui::GetWindowContentRegionWidth();
    ImVec2 uv0(0,1); // flip y axis coordinates
    ImVec2 uv1(1,0);
    for (int i=0;i<3;i++){

        ImGui::SameLine(windowWidth - width + heartSize.x * i);
        int texIndex = heartFull;
        if (i*2+1 == health){
            texIndex = heartHalf;
        } else if (i*2 > health){
            texIndex = heartEmpty;
        }
        Texture* tex = heartIcons[texIndex].get();
        ImGui::Image(tex->getNativeTexturePtr(),{heartSize.x,heartSize.y}, uv0, uv1);
    }

    // draw Score
    ImGui::PushID(1);
    auto scoreStr = std::to_string(score);
    ImGui::Text("Score"); ImGui::SameLine();
    width = ImGui::CalcTextSize(scoreStr.c_str()).x;
    ImGui::SetCursorPosX(windowWidth - width); // align right
    ImGui::Text(scoreStr.c_str());
    ImGui::PopID();

    // Draw powerbar
    ImGui::Text("Power"); ImGui::SameLine();
    width = powerbarSize.x;
    ImGui::SetCursorPosX(windowWidth - width); // align right
    // Draw background
    ImGui::Image(powerbar->getNativeTexturePtr(),{powerbarSize.x,powerbarSize.y}, uv0, uv1); ImGui::SameLine();
    float border=3;
    auto innerSize = powerbarSize - glm::vec2(border*2,border*2);
    ImGui::SetCursorPosX(windowWidth - width + border); // align right
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + border); // move down
    // scale/clip inner bar
    innerSize.x *= power;
    uv1.x *= power;
    ImVec4 tintColor(0,1,0,1);
    ImGui::Image(powerbar->getNativeTexturePtr(),{innerSize.x,innerSize.y}, uv0, uv1, tintColor);
    ImGui::PopFont();
    ImGui::End();

}

void Hero::guiInventory() {
    ImVec2 pos = {0,0};
    auto cond = ImGuiCond_Always;
    ImVec2 pivot = {0,0};
    ImGui::SetNextWindowPos(pos, cond, pivot);
    ImVec2 size = {180, 107};
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

        if (count == 0 || count %3 != 0)
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

void Hero::setCamera(std::shared_ptr<CameraComponent> cam) {
    this->cam = cam;
}
