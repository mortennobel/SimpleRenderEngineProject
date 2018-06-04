#include <iostream>

#include "sre/Texture.hpp"
#include "sre/Renderer.hpp"
#include "sre/Material.hpp"
#include "ParticleSystem.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sre/SDLRenderer.hpp>
#include <sre/impl/GL.hpp>
#include <glm/gtc/random.hpp>
#include <vector>

using namespace sre;

class ParticleSystemExample{
public:
    ParticleSystemExample(){
        r.init();

        textures.push_back(Texture::getSphereTexture());
        textures.push_back(Texture::getWhiteTexture());
        textures.push_back(Texture::create().withFile("particle-pack-01/flash01.png").build());
        textures.push_back(Texture::create().withFile("particle-pack-01/shockwave01.png").build());
        textures.push_back(Texture::create().withFile("particle-pack-01/smoke01.png").build());

        for (auto& t:textures){
            textureNames.push_back(t->getName().c_str());
        }

        particleSystem = std::make_shared<ParticleSystem>(500,textures[0]);
        particleSystem->gravity = {0,-.2,0};

        camera.lookAt(eye,at,{0,1,0});
        camera.setPerspectiveProjection(fov,near,far);

        mesh = Mesh::create()
                .withSphere()
                .build();

        planeMesh = Mesh::create()
                .withCube(10)
                .build();

        worldLights.addLight(Light::create()
                                     .withDirectionalLight(glm::normalize(glm::vec3(1,1,1)))
                                     .build());

        // Add fake shadows
        worldLights.addLight(Light::create()
                                     .withPointLight(p1-glm::vec3(0,0.8,0))
                                     .withColor({-3.0f,-3.0f,-3.0f})
                                     .withRange(4)
                                     .build());
        worldLights.addLight(Light::create()
                                     .withPointLight(p2-glm::vec3(0,0.8,0))
                                     .withColor({-3.0f,-3.0f,-3.0f})
                                     .withRange(4)
                                     .build());

        mat1 = Shader::getStandardBlinnPhong()->createMaterial();
        mat1->setColor({1,1,1,1});
        mat1->setSpecularity({0,0,0,0});

        mat2 = Shader::getStandardBlinnPhong()->createMaterial();
        mat2->setColor({1,0,0,1});
        mat2->setSpecularity({0,0,0,0});

        matPlane = Shader::getStandardBlinnPhong()->createMaterial();
        matPlane->setColor({1,1,1,1});
        matPlane->setSpecularity({0,0,0,0});


        r.frameUpdate = [&](float deltaTime){
            update(deltaTime);
        };
        r.frameRender = [&](){
            render();
        };
        updateApperance();
        updateEmit();

        r.startEventLoop();
    }

    void particleSystemGUI(){
        if (ImGui::CollapsingHeader("Particle System")){
            ImGui::DragFloat("EmissionRate", &particleSystem->emissionRate,0.1f,0,200);
            ImGui::DragFloat("Life time", &particleSystem->lifeSpan,0.1f,0,100);
            ImGui::DragFloat3("Gravity", &particleSystem->gravity.x,.1f,-50,50);
            ImGui::Checkbox("running",&particleSystem->running);
            ImGui::Checkbox("visible",&particleSystem->visible);
            ImGui::Checkbox("emitting",&particleSystem->emitting);
            ImGui::Text("Active particles: %i",particleSystem->getActiveParticles());
            bool changedApperance = ImGui::ColorEdit4("Color from", &colorFrom.x);
            changedApperance     |= ImGui::ColorEdit4("Color to", &colorTo.x);

            changedApperance |= ImGui::DragFloat("Size from", &sizeFrom,1,0.1,500);
            changedApperance |= ImGui::DragFloat("Size to", &sizeTo,1,0.1,500);
            if (changedApperance){
                updateApperance();
            }
            bool changedTex = ImGui::Combo("Texture",&selectedTexture, textureNames.data(),textureNames.size());
            if (changedTex){
                particleSystem->material->setTexture(textures[selectedTexture]);
            }
            bool changedEmit = ImGui::DragFloat3("Emit pos",&emitPosition.x);
            changedEmit |= ImGui::DragFloat("Emit velocity",&emitVelocity);
            changedEmit |= ImGui::DragFloat("Emit init rotation",&emitRotation);
            changedEmit |= ImGui::DragFloat("Emit angular velocity",&emitAngularVelocity);
            if (changedEmit){
                updateEmit();
            }
        }
    }

    void updateApperance(){
        particleSystem->updateAppearance = [&](const Particle& p){
            p.color = glm::mix(colorFrom, colorTo, p.normalizedAge);
            p.size = glm::mix(sizeFrom, sizeTo, p.normalizedAge);
        };
    }

    void updateEmit(){
        particleSystem->emitter = [&](Particle& p){
            p.position = emitPosition;
            p.velocity = glm::sphericalRand(emitVelocity);
            p.rotation = emitRotation;
            p.angularVelocity = emitAngularVelocity;
            p.size = 50;
        };
    }

    void cameraGUI(){
        if (ImGui::CollapsingHeader("Camera")){
            ImGui::Checkbox("Perspective projection", &perspective);
            if (perspective){
                ImGui::DragFloat("FOV", &fov,1,1,179);
            } else {
                ImGui::DragFloat("OrthoSize", &orthoSize,0.1,0.1,10);
            }
            ImGui::DragFloat("Near", &near,0.1,-10,10);
            ImGui::DragFloat("Far", &far,0.1,0.1,100);
            if (perspective){
                camera.setPerspectiveProjection(fov,near,far);
            } else {
                camera.setOrthographicProjection(orthoSize,near,far);
            }
            ImGui::DragFloat3("eye", &eye.x,0.1,-10,10);
            ImGui::DragFloat3("at", &at.x,0.1,-10,10);
            camera.lookAt(eye,at,{0,1,0});
        }

    }

    void update(float deltaTime){
        particleSystem->update(deltaTime);
    }

    void render(){
        auto rp = RenderPass::create()
                .withCamera(camera)
                .withWorldLights(&worldLights)
                .withClearColor(true,{1,0,0,1})
                .build();

        rp.draw(mesh, pos1, mat1);
        rp.draw(mesh, pos2, mat2);
        rp.draw(planeMesh, glm::translate(glm::vec3{0,-1.0f,0})*glm::scale(glm::vec3{1,.01f,1}), matPlane);

        particleSystem->draw(rp);

        ImGui::Begin("Settings");
        particleSystemGUI();
        cameraGUI();
        ImGui::End();
    }
private:
    SDLRenderer r;
    Camera camera;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Mesh> planeMesh;
    WorldLights worldLights;



    // camera properties
    bool perspective=true;
    float fov = 60;
    float near = 0.1;
    float far = 50;
    float orthoSize = 2;

    glm::vec3 eye = {0,0,-10};
    glm::vec3 at = {0,0,0};

    std::shared_ptr<Material> mat1;
    std::shared_ptr<Material> mat2;
    std::shared_ptr<Material> matPlane;

    glm::vec3 p1 =  {-1,0,0};
    glm::vec3 p2 =  {1,0,0};
    glm::mat4 pos1 = glm::translate(glm::mat4(1),p1);
    glm::mat4 pos2 = glm::translate(glm::mat4(1),p2);

    glm::vec4 colorFrom = {1,1,1,1};
    glm::vec4 colorTo = {1,1,1,0};
    float sizeFrom = 50;
    float sizeTo = 50;
    int selectedTexture = 0;
    std::vector<std::shared_ptr<sre::Texture>> textures;
    std::vector<const char*> textureNames;

    glm::vec3 emitPosition = {0,3,0};
    float emitVelocity= 1;
    float emitRotation = 10;
    float emitAngularVelocity = 10;

    std::shared_ptr<ParticleSystem> particleSystem;
};

int main() {
    new ParticleSystemExample();

    return 0;
}

