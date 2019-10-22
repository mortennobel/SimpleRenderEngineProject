/*
* Copyright (c) 2006-2016 Erin Catto http://www.box2d.org
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

#include <iostream>
#include "sre/SDLRenderer.hpp"
#include "sre/Mesh.hpp"
#include "Test.h"
#include <sre/Inspector.hpp>
#include "sre/Resource.hpp"
#include <glm/ext.hpp>

//
struct UIState
{
	bool showMenu;
};

//
namespace
{
	sre::SDLRenderer mainWindow;
	UIState ui;

	bool showInspector = false;

	int32_t testIndex = 0;
	int32_t testSelection = 0;
	int32_t testCount = 0;
	b2Vec2 cursorPos;
	TestEntry* entry;
	Test* test;
	Settings settings;
	bool rightMouseDown;
	b2Vec2 lastp;
	std::shared_ptr<sre::Mesh> lineMesh;
	std::shared_ptr<sre::Mesh> triangleMesh;
	std::shared_ptr<sre::Material> meshMaterial;

	float frameTime = 0;
}

//
static void sCreateUI()
{
	ui.showMenu = true;

	// Init UI
	const char* fontPath = "Data/DroidSans.ttf";
	ImGui::GetIO().Fonts->AddFontFromFileTTF(fontPath, 15.f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = style.GrabRounding = style.ScrollbarRounding = 2.0f;
	style.FramePadding = ImVec2(4, 2);
	style.DisplayWindowPadding = ImVec2(0, 0);
	style.DisplaySafeAreaPadding = ImVec2(0, 0);
}

//
static void sResizeWindow(int width, int height)
{
	g_camera.m_width = width;
	g_camera.m_height = height;
}

//
static void sKeyCallback(int key, int scancode, bool down, int mods)
{

	//ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods);
	bool keys_for_ui = ImGui::GetIO().WantCaptureKeyboard;
	if (keys_for_ui)
		return;

	if (down)
	{
		switch (key)
		{
		case SDLK_ESCAPE:
			// Quit
			//glfwSetWindowShouldClose(mainWindow, GL_TRUE);
			break;

		case SDLK_LEFT:
			// Pan left
			if ((mods & KMOD_CTRL) != 0)
			{
				b2Vec2 newOrigin(2.0f, 0.0f);
				test->ShiftOrigin(newOrigin);
			}
			else
			{
				g_camera.m_center.x -= 0.5f;
			}
			break;

		case SDLK_RIGHT:
			// Pan right
			if ((mods & KMOD_CTRL) != 0)
			{
				b2Vec2 newOrigin(-2.0f, 0.0f);
				test->ShiftOrigin(newOrigin);
			}
			else
			{
				g_camera.m_center.x += 0.5f;
			}
			break;

		case SDLK_DOWN:
			// Pan down
			if ((mods & KMOD_CTRL) != 0)
			{
				b2Vec2 newOrigin(0.0f, 2.0f);
				test->ShiftOrigin(newOrigin);
			}
			else
			{
				g_camera.m_center.y -= 0.5f;
			}
			break;

		case SDLK_UP:
			// Pan up
			if ((mods & KMOD_CTRL) != 0)
			{
				b2Vec2 newOrigin(0.0f, -2.0f);
				test->ShiftOrigin(newOrigin);
			}
			else
			{
				g_camera.m_center.y += 0.5f;
			}
			break;

		case SDLK_HOME:
			// Reset view
			g_camera.m_zoom = 1.0f;
			g_camera.m_center.Set(0.0f, 20.0f);
			break;

		case SDLK_z:
			// Zoom out
			g_camera.m_zoom = b2Min(1.1f * g_camera.m_zoom, 20.0f);
			break;

		case SDLK_x:
			// Zoom in
			g_camera.m_zoom = b2Max(0.9f * g_camera.m_zoom, 0.02f);
			break;

		case SDLK_r:
			// Reset test
			delete test;
			test = entry->createFcn();
			break;

		case SDLK_SPACE:
			// Launch a bomb.
			if (test)
			{
				test->LaunchBomb();
			}
			break;

		case SDLK_p:
			// Pause
			settings.pause = !settings.pause;
			break;

		case SDLK_LEFTBRACKET:
			// Switch to previous test
			--testSelection;
			if (testSelection < 0)
			{
				testSelection = testCount - 1;
			}
			break;

		case SDLK_RIGHTBRACKET:
			// Switch to next test
			++testSelection;
			if (testSelection == testCount)
			{
				testSelection = 0;
			}
			break;

		case SDLK_TAB:
			ui.showMenu = !ui.showMenu;

		default:
			if (test)
			{
				test->Keyboard(key);
			}
		}
	}
	else if (!down)
	{
		test->KeyboardUp(key);
	}
	// else GLFW_REPEAT

}

//
static void sMouseButton(int32 button, bool down, int32 mods)
{
	double xd = cursorPos.x, yd = cursorPos.y;
	b2Vec2 ps((float32)xd, (float32)yd);

	// Use the mouse to move things around.
	if (button == SDL_BUTTON_LEFT)
	{

		b2Vec2 pw = g_camera.ConvertScreenToWorld(ps);
		if (down)
		{
			if (mods == SDLK_LSHIFT || mods == SDLK_RSHIFT)
			{
				test->ShiftMouseDown(pw);
			}
			else
			{
				test->MouseDown(pw);
			}
		}
		
		if (!down)
		{
			test->MouseUp(pw);
		}
	}
	else if (button == SDL_BUTTON_RIGHT)
	{
		if (down)
		{	
			lastp = g_camera.ConvertScreenToWorld(ps);
			rightMouseDown = true;
		}

		if (!down)
		{
			rightMouseDown = false;
		}
	}
}

//
static void sMouseMotion(double xd, double yd)
{

	b2Vec2 ps((float)xd, (float)yd);
	cursorPos = ps;
	b2Vec2 pw = g_camera.ConvertScreenToWorld(ps);
	test->MouseMove(pw);
	
	if (rightMouseDown)
	{
		b2Vec2 diff = pw - lastp;
		g_camera.m_center.x -= diff.x;
		g_camera.m_center.y -= diff.y;
		lastp = g_camera.ConvertScreenToWorld(ps);
	}
}

//
static void sScrollCallback(double dx, double dy)
{
	bool mouse_for_ui = ImGui::GetIO().WantCaptureMouse;

	if (!mouse_for_ui)
	{
		if (dy > 0)
		{
			g_camera.m_zoom /= 1.1f;
		}
		else
		{
			g_camera.m_zoom *= 1.1f;
		}
	}
}

//
static void sRestart()
{
	delete test;
	entry = g_testEntries + testIndex;
	test = entry->createFcn();
}

//
static void sSimulate()
{
	//glEnable(GL_DEPTH_TEST);
	test->Step(&settings);

	test->DrawTitle(entry->name);
	//glDisable(GL_DEPTH_TEST);

	if (testSelection != testIndex)
	{
		testIndex = testSelection;
		delete test;
		entry = g_testEntries + testIndex;
		test = entry->createFcn();
		g_camera.m_zoom = 1.0f;
		g_camera.m_center.Set(0.0f, 20.0f);
	}
}

//
static bool sTestEntriesGetName(void*, int idx, const char** out_name)
{
	*out_name = g_testEntries[idx].name;
	return true;
}

//
static void sInterface()
{
	int menuWidth = 200;
	if (ui.showMenu)
	{
		ImGui::SetNextWindowPos(ImVec2((float)g_camera.m_width - menuWidth - 10, 10));
		ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)g_camera.m_height - 20));
		ImGui::Begin("Testbed Controls", &ui.showMenu, ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoCollapse);
		ImGui::PushAllowKeyboardFocus(false); // Disable TAB

		ImGui::PushItemWidth(-1.0f);

		ImGui::Text("Test");
		if (ImGui::Combo("##Test", &testIndex, sTestEntriesGetName, NULL, testCount, testCount))
		{
			delete test;
			entry = g_testEntries + testIndex;
			test = entry->createFcn();
			testSelection = testIndex;
		}
		ImGui::Separator();

		ImGui::Text("Vel Iters");
		ImGui::SliderInt("##Vel Iters", &settings.velocityIterations, 0, 50);
		ImGui::Text("Pos Iters");
		ImGui::SliderInt("##Pos Iters", &settings.positionIterations, 0, 50);
		ImGui::Text("Hertz");
		ImGui::SliderFloat("##Hertz", &settings.hz, 5.0f, 120.0f, "%.0f hz");
		ImGui::PopItemWidth();

		ImGui::Checkbox("Sleep", &settings.enableSleep);
		ImGui::Checkbox("Warm Starting", &settings.enableWarmStarting);
		ImGui::Checkbox("Time of Impact", &settings.enableContinuous);
		ImGui::Checkbox("Sub-Stepping", &settings.enableSubStepping);

		ImGui::Separator();

		ImGui::Checkbox("Shapes", &settings.drawShapes);
		ImGui::Checkbox("Joints", &settings.drawJoints);
		ImGui::Checkbox("AABBs", &settings.drawAABBs);
		ImGui::Checkbox("Contact Points", &settings.drawContactPoints);
		ImGui::Checkbox("Contact Normals", &settings.drawContactNormals);
		ImGui::Checkbox("Contact Impulses", &settings.drawContactImpulse);
		ImGui::Checkbox("Friction Impulses", &settings.drawFrictionImpulse);
		ImGui::Checkbox("Center of Masses", &settings.drawCOMs);
		ImGui::Checkbox("Statistics", &settings.drawStats);
		ImGui::Checkbox("Profile", &settings.drawProfile);

		ImVec2 button_sz = ImVec2(-1, 0);
		if (ImGui::Button("Pause (P)", button_sz))
			settings.pause = !settings.pause;

		if (ImGui::Button("Single Step", button_sz))
			settings.singleStep = !settings.singleStep;

		if (ImGui::Button("Restart (R)", button_sz))
			sRestart();

		ImGui::PopAllowKeyboardFocus();
		ImGui::End();
	}
}

//
int main(int, char**)
{
	std::cout << "https://github.com/mortennobel/SimpleRenderEngineProject" << std::endl;

	g_camera.m_width = 1024;
	g_camera.m_height = 640;

	char title[64];
	sprintf(title, "Box2D Testbed Version %d.%d.%d", b2_version.major, b2_version.minor, b2_version.revision);
	mainWindow.setWindowTitle(title);
	mainWindow.setWindowSize({g_camera.m_width,g_camera.m_height});
	mainWindow.init()
            .withSdlInitFlags(SDL_INIT_EVERYTHING)
			.withSdlWindowFlags(SDL_WINDOW_OPENGL);


	std::string vertexShaderSource =  R"(#version 330
in vec4 posxyzw;
in vec4 color;
out vec4 vColor;

#pragma include "global_uniforms_incl.glsl"

#pragma include "sre_utils_incl.glsl"

void main(void) {
    gl_Position = g_projection * g_view * g_model * posxyzw;
    vColor = color;
}
)";
	std::string fragmentShaderSource = R"(#version 330
out vec4 fragColor;
in vec4 vColor;

#pragma include "sre_utils_incl.glsl"

void main(void)
{
    fragColor = vColor;
}
)";

    auto filenameVertex = "vertexShaderSource";
    sre::Resource::set(filenameVertex, vertexShaderSource);
    auto filenameFragment = "fragmentShaderSource";
    sre::Resource::set(filenameFragment, fragmentShaderSource);


	meshMaterial = sre::Shader::create()
            .withSourceResource(filenameVertex, sre::ShaderType::Vertex)
			.withSourceResource(filenameFragment,sre::ShaderType::Fragment)
			.withBlend(sre::BlendType::AlphaBlending)
			.withDepthTest(false)
			.withDepthWrite(false)
			.build()->createMaterial();
	std::vector<glm::vec3> pos;
	std::vector<glm::vec4> color;
	lineMesh = sre::Mesh::create()
			.withAttribute("posxyzw",pos)
			.withAttribute("color",color)
			.withMeshTopology(sre::MeshTopology::Lines)
			.build();
	triangleMesh = sre::Mesh::create()
			.withAttribute("posxyzw",pos)
			.withAttribute("color",color)
			.build();

	mainWindow.mouseEvent = [](SDL_Event& e){
		if (e.type == SDL_MOUSEWHEEL){
			sScrollCallback(e.wheel.x,e.wheel.y);
		} else if (e.type == SDL_MOUSEMOTION){
			sMouseMotion(e.motion.x,e.motion.y);
		} else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP){
			bool down = e.type == SDL_MOUSEBUTTONDOWN;
			sMouseButton(e.button.button, down, 0);
		}
		if (e.button.button==SDL_BUTTON_RIGHT){
			showInspector = true;
		}
	};
	mainWindow.keyEvent = [](SDL_Event& e){
		bool down = e.type == SDL_KEYDOWN;
		sKeyCallback(e.key.keysym.sym,e.key.keysym.scancode,down, e.key.keysym.mod);

	};

	mainWindow.frameUpdate = [](float deltaTime){
		frameTime = deltaTime;

	};

	mainWindow.frameRender = [&](){

		glm::mat4 proj;
		g_camera.BuildProjectionMatrix(&proj[0][0], 0.0f);

		sre::Camera cam;
		cam.setProjectionTransform(proj);

		auto rp = sre::RenderPass::create()
				.withCamera(cam)
				.build();

		sSimulate();
		sInterface();

		auto pos = g_debugDraw.getTriangles();
		auto color = g_debugDraw.getTriangleColors();
		triangleMesh->update()
				.withAttribute("posxyzw",pos)
				.withAttribute("color",color)
				.build();
		rp.draw(triangleMesh,glm::mat4(1),meshMaterial);

		pos = g_debugDraw.getLines();
		color = g_debugDraw.getLineColors();
		lineMesh->update()
				.withAttribute("posxyzw",pos)
				.withAttribute("color",color)
				.build();


		rp.draw(lineMesh,glm::mat4(1),meshMaterial);
		g_debugDraw.clear();
		static sre::Inspector inspector;
		inspector.update();
		if (showInspector){
			inspector.gui();
		}
	};

	g_debugDraw.Create();

	sCreateUI();

	testCount = 0;
	while (g_testEntries[testCount].createFcn != NULL)
	{
		++testCount;
	}

	testIndex = b2Clamp(testIndex, 0, testCount - 1);
	testSelection = testIndex;

	entry = g_testEntries + testIndex;
	test = entry->createFcn();

	mainWindow.startEventLoop();

	if (test)
	{
		delete test;
		test = nullptr;
	}

	g_debugDraw.Destroy();


	return 0;
}
