//
// Created by Morten Nobel Jørgensen on 2018-10-30.
//

#include "Component.hpp"

class RPG;

class MainMenuComponent : public Component {
public:
    explicit MainMenuComponent(GameObject *gameObject);
    void onGui() override;
    void setRPG(RPG* rpg);
private:
    void playPressed();
    void settingsPressed();

    RPG* rpg;
};
