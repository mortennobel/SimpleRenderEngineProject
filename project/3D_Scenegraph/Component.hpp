//
// Created by Morten Nobel Jørgensen on 2018-11-06.
//

#pragma once

// forward declaration
class GameObject;

class Component {
public:
    virtual ~Component() = default;
    GameObject* getGameObject();

    // invoked on start
    virtual void onStart();

    // invoked on object destruction
    virtual void onDestroy();


    virtual void debugGUI();

protected:
    Component(GameObject* gameObject);
    GameObject *gameObject = nullptr;

    friend class GameObject;
};