//
// Created by Morten Nobel Jørgensen on 2018-11-11.
//

#pragma once

#include "LinearMath/btIDebugDraw.h"
#include <vector>
#include "glm/glm.hpp"
#include "sre/Color.hpp"
#include "sre/RenderPass.hpp"

class BulletDebugDrawSRE final : public btIDebugDraw {
public:
    void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;

    void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime,
                          const btVector3 &color) override;

    void reportErrorWarning(const char *warningString) override;

    void draw3dText(const btVector3 &location, const char *textString) override;

    void setDebugMode(int debugMode) override;

    int getDebugMode() const override;

    void newFrame();
    void render(sre::RenderPass& renderPass);
private:
    DebugDrawModes mode = DBG_DrawWireframe;
    std::vector<glm::vec3> linePositions;
    std::vector<sre::Color> lineColors;
};


