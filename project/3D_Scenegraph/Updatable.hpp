//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#pragma once

class Updatable {
public:
    virtual void update(float time) = 0;
};