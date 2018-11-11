//
// Created by Morten Nobel Jørgensen on 2018-11-11.
//

#include "BulletDebugDrawSRE.hpp"
#include <iostream>

void BulletDebugDrawSRE::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
    linePositions.emplace_back(from.x(), from.y(), from.z());
    linePositions.emplace_back(to.x(), to.y(), to.z());
    lineColors.emplace_back(color.x(), color.y(), color.z());
    lineColors.emplace_back(color.x(), color.y(), color.z());
}

void BulletDebugDrawSRE::render(sre::RenderPass &renderPass) {
    auto pos1 = linePositions[0];
    auto pos2 = linePositions[1];
    renderPass.drawLines(linePositions, {0.0f,1.0f,0.0f});
}

void BulletDebugDrawSRE::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance,
                                          int lifeTime, const btVector3 &color) {

}

void BulletDebugDrawSRE::reportErrorWarning(const char *warningString) {
    std::cout << warningString << std::endl;
}

void BulletDebugDrawSRE::draw3dText(const btVector3 &location, const char *textString) {

}

void BulletDebugDrawSRE::setDebugMode(int debugMode) {
    mode = static_cast<DebugDrawModes>(debugMode);
}

int BulletDebugDrawSRE::getDebugMode() const {
    return mode;
}

void BulletDebugDrawSRE::newFrame() {
    linePositions.clear();
    lineColors.clear();
}
