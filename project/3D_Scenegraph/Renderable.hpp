#pragma once

#include <sre/RenderPass.hpp>

class Renderable {
public:
    virtual void draw(sre::RenderPass* renderPass) = 0;
};