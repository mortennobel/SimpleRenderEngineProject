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

// Captures debug information from Box2D as lines (polygon fill and color is discarded)
class ScriptableSphereObject {
private:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
public:
    int x;
    int z;

    float pos_x;
    float pos_y;
    float pos_z;

    string objectName;
    ScriptableSphereObject(std::shared_ptr<Mesh> mesh,std::shared_ptr<Material> material, int x, int z)
    :mesh(mesh), material(material), x(x), z(z)
    {
        pos_x = x;
        pos_y = 0;
        pos_z = z;
        objectName = string("o")+to_string(x)+"_"+to_string(z);
    }

    void draw(RenderPass& renderPass){
        renderPass.draw(mesh, glm::translate(glm::vec3{pos_x,pos_y,pos_z}), material);
    }
};

class LuaExample {
public:
    LuaExample()
    {
        r.init();
        r.setWindowTitle("Lua example");

        l.addLight(Light::create().withDirectionalLight({1,1,1}).withColor({1,1,1}).build());

        auto mat = Shader::getStandardPhong()->createMaterial();
        mat->setColor({1,0,0,1});
        auto sphere = Mesh::create().withSphere().build();

        lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);

        // callback set title
        lua.set_function("setTitle", [&](sol::this_state, sol::variadic_args va){
            string s = va[0];
            r.setWindowTitle(s);
        });


        updateLuaScript();

        // create objects
        for (int x = -5;x<=5;x++){
            for (int y = -5;y<=5;y++) {
                objects.emplace_back(sphere, mat, x, y);
            }
        }

        camera.setPerspectiveProjection(45,0.1,100);
        camera.lookAt({0,15,-20},{0,0,0},{0,1,0});

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
        sol::function fn = lua["updatepos"];    // get lua function update pos
        updatepos = fn;                         // get C++ functional reference to lua script
    }

    void update(float deltaTime){
        static float totalTime = 0;
        totalTime += deltaTime;
        for (auto & o : objects){
            o.pos_y = (float)updatepos(o.x,o.z,totalTime);
        }
    }

    void render(){
        RenderPass rp = RenderPass::create()
                .withCamera(camera)
                .withWorldLights(&l)
                .build();
        for (auto & o :  objects){
            o.draw(rp);
        }


        ImGui::InputTextMultiline("Lua script", luaScript , 2048);

        if (ImGui::Button("Update")){
            updateLuaScript();
        }
    }
private:
    static const int luaScriptSize = 2048;
    char luaScript[luaScriptSize] =
            "-- Function evaluated for each pos\n"
            "function updatepos (x, z, time) \n"
            " return math.sin(x + z*9 + time) \n"
            "end\n"
            "-- Remove '--' to see callback\n"
            "-- setTitle('callback to setTitle')";

    std::vector<ScriptableSphereObject> objects;
    SDLRenderer r;
    WorldLights l;
    Camera camera;
    sol::state lua;
    std::function<double(int, int, double)> updatepos;
};

int main() {
    std::cout << "https://github.com/mortennobel/SimpleRenderEngineProject" << std::endl;
    new LuaExample();

    return 0;
}