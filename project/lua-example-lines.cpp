#include <iostream>
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include "sre/Texture.hpp"
#include "sre/Material.hpp"
#include "sre/Inspector.hpp"
#include "glm/gtc/random.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"
#include "glm/ext.hpp"

// lua headers
#include "sol.hpp"

using namespace sre;
using namespace std;
using namespace sol;

class LuaExample {
public:
    LuaExample()
    {
        r.init();
        r.setWindowTitle("Lua example");

        objPosition = {5.5f,0,0};

        mat = Shader::getUnlit()->createMaterial();
        mat->setColor({1,0,0,1});
        mesh = Mesh::create().withSphere().build();

        lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);

        // Register callback functions
        lua.set_function("pi", [&](sol::this_state state, sol::variadic_args va){
            return glm::pi<double>();
        });
        lua.set_function("fmod", [&](sol::this_state state, sol::variadic_args va){
            double numer = va[0];
            double denum = va[1];
            return fmod(numer,denum);
        });
        lua.set_function("remainder", [&](sol::this_state state, sol::variadic_args va){
            double s = va[0];
            double denum = va[1];
            return remainder(s,denum);
        });
        lua.set_function("abs", [&](sol::this_state state, sol::variadic_args va){
            double s = va[0];
            return abs(s);
        });
        lua.set_function("sin", [&](sol::this_state state, sol::variadic_args va){
            double s = va[0];
            return sin(s);
        });
        lua.set_function("cos", [&](sol::this_state state, sol::variadic_args va){
            double s = va[0];
            return cos(s);
        });
        lua.set_function("pow", [&](sol::this_state state, sol::variadic_args va){
            double s = va[0];
            double p = va[1];
            return pow(s,p);
        });
        updateLuaScript();

        // init objects
        for (float x = -5;x<=5;x=x+0.01f){
            positions.push_back({x,0,0});
        }

        camera.setOrthographicProjection(5,-1,1);

        r.frameUpdate = [&](float deltaTime){
            update(deltaTime);
        };
        r.frameRender = [&](){
            render();
        };

        r.startEventLoop();
    }

    void updateLuaScript() {
        lua.script(luaScript);                  // evaluate lua script
        sol::function fn = lua["easing"];    // get lua function update pos
        updatepos = fn;                         // get C++ functional reference to lua script
    }

    void update(float deltaTime){
        static float totalTime = 0;
        totalTime += deltaTime;
        try {
            if (error.size()==0){
                objPosition.y = (float) updatepos(totalTime);
            }
        } catch (sol::error err){
            error = err.what();
        }
    }

    void render(){
        try {
            if (error.size()==0){
                int i = 0;
                for (float x = -5; x <= 5; x = x + 0.01f) {
                    positions[i] = {x, (float) updatepos(x), 0};
                    i++;
                }
            }
        } catch (sol::error err){
            error = err.what();
        }

        RenderPass rp = RenderPass::create()
                .withCamera(camera)
                .build();

        // draw grid
        for (int i=-5;i<=5;i++){
            Color color = (i==0?Color{1.0f,1.0f,1.0f,1.0f}:Color{0.2f,0.2f,0.2f,1.0f});
            rp.drawLines({{i,-5,0},{i,5,0}},color, MeshTopology::LineStrip);
            rp.drawLines({{-5,i,0},{5,i,0}},color, MeshTopology::LineStrip);
        }

        rp.drawLines(positions,{1.0,1.0,1.0,1.0}, MeshTopology::LineStrip);

        rp.draw(mesh, glm::translate(objPosition)*glm::scale(glm::vec3{0.5f,0.5f,0.5f}), mat);

        ImGui::InputTextMultiline("Lua script", luaScript , luaScriptSize);
        if (error.size() > 0){
            ImGui::LabelText("Error", error.c_str());
        }
        if (ImGui::Button("Update")){
            try {
                updateLuaScript();
                error = "";
            } catch (sol::error err){
                error = err.what();
            }
        }
    }
private:
    static const int luaScriptSize = 2048;
    // initial script
    char luaScript[luaScriptSize] =
            "-- C functions exposed:\n"
            "-- fmod(a,b), remainder(a,b), abs(a), sin(a), cos(a), pow(a), pi()\n"
            "function easing (t) \n"
            " return t \n"
            "end";

    std::vector<glm::vec3> positions;

    glm::vec3 objPosition;

    std::shared_ptr<sre::Mesh> mesh;
    std::shared_ptr<sre::Material> mat;
    SDLRenderer r;
    Camera camera;
    std::string error;
    sol::state lua;
    std::function<double(double)> updatepos;
};

int main() {
    std::cout << "https://github.com/mortennobel/SimpleRenderEngineProject" << std::endl;
    new LuaExample();

    return 0;
}