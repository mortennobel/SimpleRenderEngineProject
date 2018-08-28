/*
* Copyright (c) 2006-2013 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "DebugDraw.h"


#include <stdio.h>
#include <stdarg.h>
#include <glm/gtc/constants.hpp>
#include <glm/vec4.hpp>

#include "imgui.h"

#define BUFFER_OFFSET(x)  ((const void*) (x))

DebugDraw g_debugDraw;
Camera g_camera;

//
b2Vec2 Camera::ConvertScreenToWorld(const b2Vec2& ps)
{
    float32 w = float32(m_width);
    float32 h = float32(m_height);
	float32 u = ps.x / w;
	float32 v = (h - ps.y) / h;

	float32 ratio = w / h;
	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= m_zoom;

	b2Vec2 lower = m_center - extents;
	b2Vec2 upper = m_center + extents;

	b2Vec2 pw;
	pw.x = (1.0f - u) * lower.x + u * upper.x;
	pw.y = (1.0f - v) * lower.y + v * upper.y;
	return pw;
}

//
b2Vec2 Camera::ConvertWorldToScreen(const b2Vec2& pw)
{
	float32 w = float32(m_width);
	float32 h = float32(m_height);
	float32 ratio = w / h;
	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= m_zoom;

	b2Vec2 lower = m_center - extents;
	b2Vec2 upper = m_center + extents;

	float32 u = (pw.x - lower.x) / (upper.x - lower.x);
	float32 v = (pw.y - lower.y) / (upper.y - lower.y);

	b2Vec2 ps;
	ps.x = u * w;
	ps.y = (1.0f - v) * h;
	return ps;
}

// Convert from world coordinates to normalized device coordinates.
// http://www.songho.ca/opengl/gl_projectionmatrix.html
void Camera::BuildProjectionMatrix(float32* m, float32 zBias)
{
	float32 w = float32(m_width);
	float32 h = float32(m_height);
	float32 ratio = w / h;
	b2Vec2 extents(ratio * 25.0f, 25.0f);
	extents *= m_zoom;

	b2Vec2 lower = m_center - extents;
	b2Vec2 upper = m_center + extents;

	m[0] = 2.0f / (upper.x - lower.x);
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;

	m[4] = 0.0f;
	m[5] = 2.0f / (upper.y - lower.y);
	m[6] = 0.0f;
	m[7] = 0.0f;

	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = 1.0f;
	m[11] = 0.0f;

	m[12] = -(upper.x + lower.x) / (upper.x - lower.x);
	m[13] = -(upper.y + lower.y) / (upper.y - lower.y);
	m[14] = zBias;
	m[15] = 1.0f;
}



//
DebugDraw::DebugDraw()
{

}

//
DebugDraw::~DebugDraw()
{

}

//
void DebugDraw::Create()
{

}

//
void DebugDraw::Destroy()
{

}

//
void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glm::vec4 fillColor(color.r, color.g, color.b, 1.0f);
	// todo set color
	for (int i=0;i<vertexCount;i++){
		lines.push_back({vertices[i].x,vertices[i].y,0});
		lines.push_back({vertices[(i+1)%vertexCount].x,vertices[(i+1)%vertexCount].y,0});

		lineColors.push_back(fillColor);
		lineColors.push_back(fillColor);
	}
}

//
void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glm::vec4 fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);

    for (int32 i = 1; i < vertexCount - 1; ++i)
    {
		triangles.push_back({vertices[0].x, vertices[0].y, -0.1f});
		triangles.push_back({vertices[i].x, vertices[i].y, -0.1f});
		triangles.push_back({vertices[i+1].x, vertices[i+1].y, -0.1f});
        triangleColors.push_back(fillColor);
        triangleColors.push_back(fillColor);
        triangleColors.push_back(fillColor);
    }

	DrawPolygon(vertices, vertexCount, color);
}

//
void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	glm::vec4 fillColor(color.r, color.g, color.b, 1.0f);
	glm::vec3 c {center.x,center.y,0};
	for (int i=0;i<16;i++){
		float v = i*glm::two_pi<float>()/16;
		float v1 = (i+1)*glm::two_pi<float>()/16;
		lines.push_back(c+glm::vec3{sin(v ) , cos(v ) , 0} * radius);
		lines.push_back(c+glm::vec3{sin(v1) , cos(v1) , 0} * radius);
		lineColors.push_back(fillColor);
		lineColors.push_back(fillColor);
	}
}

//
void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 sinInc = sinf(k_increment);
	float32 cosInc = cosf(k_increment);
	b2Vec2 v0 = center;
	b2Vec2 r1(cosInc, sinInc);
	b2Vec2 v1 = center + radius * r1;

	glm::vec4 fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	for (int32 i = 0; i < k_segments; ++i)
	{
		// Perform rotation to avoid additional trigonometry.
		b2Vec2 r2;
		r2.x = cosInc * r1.x - sinInc * r1.y;
		r2.y = sinInc * r1.x + cosInc * r1.y;
		b2Vec2 v2 = center + radius * r2;
		triangles.push_back({v0.x,v0.y,-0.1f});
		triangles.push_back({v1.x,v1.y,-0.1f});
		triangles.push_back({v2.x,v2.y,-0.1f});
		triangleColors.push_back(fillColor);
		triangleColors.push_back(fillColor);
		triangleColors.push_back(fillColor);
		r1 = r2;
		v1 = v2;
	}

	r1.Set(1.0f, 0.0f);
	v1 = center + radius * r1;
	DrawCircle(center,radius,color);

	// Draw a line fixed in the circle to animate rotation.
	fillColor = glm::vec4 (color.r, color.g, color.b, 1.0f);
	b2Vec2 p = center + radius * axis;
	lines.push_back({center.x,center.y,0});
	lines.push_back({p.x,p.y,0});
	lineColors.push_back(fillColor);
	lineColors.push_back(fillColor);
}

//
void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	glm::vec4  fillColor (color.r, color.g, color.b, 1.0f);
	lines.push_back({p1.x,p1.y,0});
	lines.push_back({p2.x,p2.y,0});
	lineColors.push_back(fillColor);
	lineColors.push_back(fillColor);
}

//
void DebugDraw::DrawTransform(const b2Transform& xf)
{
	const float32 k_axisScale = 0.4f;
	glm::vec4 red(1.0f, 0.0f, 0.0f,1.0f);
	glm::vec4 green(0.0f, 1.0f, 0.0f,1.0f);
	b2Vec2 p1 = xf.p, p2;

	lines.push_back({p1.x,p1.y,0});
	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	lines.push_back({p2.x,p2.y,0});
	lineColors.push_back(red);
	lineColors.push_back(red);

	lines.push_back({p1.x,p1.y,0});
	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	lines.push_back({p2.x,p2.y,0});
	lineColors.push_back(green);
	lineColors.push_back(green);
}

//
void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	size /= 20;
	glm::vec4  fillColor (color.r, color.g, color.b, 1.0f);
	lines.push_back({p.x-size,p.y,0});
	lines.push_back({p.x+size,p.y,0});
	lines.push_back({p.x,p.y-size,0});
	lines.push_back({p.x,p.y+size,0});
	lineColors.push_back(fillColor);
	lineColors.push_back(fillColor);
	lineColors.push_back(fillColor);
	lineColors.push_back(fillColor);
}

//
void DebugDraw::DrawString(int x, int y, const char *string, ...)
{
	va_list arg;
	va_start(arg, string);
	ImGui::Begin("Overlay", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);

	ImGui::SetCursorPos(ImVec2(x, y));
	ImGui::TextColoredV(ImColor(230, 153, 153, 255), string, arg);
	ImGui::End();
	va_end(arg);
}

//
void DebugDraw::DrawString(const b2Vec2& pw, const char *string, ...)
{
	b2Vec2 ps = g_camera.ConvertWorldToScreen(pw);

	va_list arg;
	va_start(arg, string);
	ImGui::Begin("Overlay", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
	ImGui::SetCursorPos(ImVec2(ps.x, ps.y));
	ImGui::TextColoredV(ImColor(230, 153, 153, 255), string, arg);
	ImGui::End();
	va_end(arg);
}

//
void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{
    b2Vec2 p1 = aabb->lowerBound;
    b2Vec2 p2 = b2Vec2(aabb->upperBound.x, aabb->lowerBound.y);
    b2Vec2 p3 = aabb->upperBound;
    b2Vec2 p4 = b2Vec2(aabb->lowerBound.x, aabb->upperBound.y);

	glm::vec4  fillColor (c.r, c.g, c.b, 1.0f);
	lines.push_back({p1.x,p1.y,0});
	lines.push_back({p2.x,p2.y,0});

	lines.push_back({p2.x,p2.y,0});
	lines.push_back({p3.x,p3.y,0});

	lines.push_back({p3.x,p3.y,0});
	lines.push_back({p4.x,p4.y,0});

	lines.push_back({p4.x,p4.y,0});
	lines.push_back({p1.x,p1.y,0});
	for (int i=0;i<8;i++){
		lineColors.push_back({fillColor});
	}

}

//
void DebugDraw::Flush()
{
    /*m_triangles->Flush();
    m_lines->Flush();
    m_points->Flush();*/
}

void DebugDraw::clear() {
	lines.clear();
	lineColors.clear();

	triangles.clear();
	triangleColors.clear();
}

const std::vector<glm::vec3> &DebugDraw::getLines() {
	return lines;
}

const std::vector<glm::vec4> &DebugDraw::getLineColors() {
	return lineColors;
}

const std::vector<glm::vec3> &DebugDraw::getTriangles() const {
	return triangles;
}

const std::vector<glm::vec4> &DebugDraw::getTriangleColors() const {
	return triangleColors;
}
