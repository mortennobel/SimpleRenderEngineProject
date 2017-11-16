//
// Created by Morten Nobel-Jørgensen on 19/10/2017.
//

#include "Box2DDebugDraw.hpp"
#include "glm/gtc/random.hpp"
#include "glm/ext.hpp"

using namespace std;

Box2DDebugDraw::Box2DDebugDraw(float scale)
:scale(scale)
{
    SetFlags( b2Draw::e_shapeBit );
}

void Box2DDebugDraw::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
    for (int i=0;i<vertexCount;i++){
        lines.push_back({vertices[i].x*scale,vertices[i].y*scale,0});
        lines.push_back({vertices[(i+1)%vertexCount].x*scale,vertices[(i+1)%vertexCount].y*scale,0});
    }
}

void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
    DrawPolygon(vertices, vertexCount, color);
}

void Box2DDebugDraw::DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color) {
    glm::vec3 c {center.x*scale,center.y * scale,0};
    for (int i=0;i<16;i++){
        float v = i*glm::two_pi<float>()/16;
        float v1 = (i+1)*glm::two_pi<float>()/16;
        lines.push_back(c+glm::vec3{sin(v ) , cos(v ) , 0} * radius * scale);
        lines.push_back(c+glm::vec3{sin(v1) , cos(v1) , 0} * radius * scale);
    }
}

void Box2DDebugDraw::DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color) {
    DrawCircle(center, radius, color);
}

void Box2DDebugDraw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) {
    lines.push_back({p1.x * scale,p1.y * scale,0});
    lines.push_back({p2.x * scale,p2.y * scale,0});
}

void Box2DDebugDraw::DrawTransform(const b2Transform &xf) {}

void Box2DDebugDraw::DrawPoint(const b2Vec2 &p, float32 size, const b2Color &color) {}

void Box2DDebugDraw::clear() {
    lines.clear();
}

const std::vector<glm::vec3> &Box2DDebugDraw::getLines() {
    return lines;
}
