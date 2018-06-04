#include <iostream>
#include <vector>
#include <memory>
#include <fstream>

#include "sre/Texture.hpp"
#include "sre/Renderer.hpp"
#include "sre/Material.hpp"
#include "sre/SDLRenderer.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/ext.hpp>
#include <sre/Inspector.hpp>

using namespace sre;
using namespace std;

class Node;

std::shared_ptr<Node> cameraNode;

class Node : public std::enable_shared_from_this<Node> {
public:
    Node() {}

    Node(Node *parent) : parent(parent) {}

    std::string name;
    glm::vec3 position = glm::vec3(0,0,0);
    glm::vec3 rotationEuler = glm::vec3(0,0,0);
    glm::vec3 scale = glm::vec3(1,1,1);
    std::vector<std::shared_ptr<sre::Mesh>> meshes;
    std::vector<std::shared_ptr<sre::Material>> materials;

    std::vector<std::shared_ptr<Node>> children;
    Node* parent = nullptr;
    bool visualizeSkeleton = false;

    glm::mat4 localToWorld(){
        // compute trs
        auto rotationEulerRadian = glm::radians(rotationEuler);
        auto res = glm::translate(position)
                   * glm::eulerAngleXYZ(rotationEulerRadian.x,rotationEulerRadian.y,rotationEulerRadian.z)
                   * glm::scale(scale);
        if (parent){
            res = parent->localToWorld() * res;
        }
        return res;
    }

    void gui(int level = 0){
        ImGui::PushID(this);
        ImGui::Indent(level*20);

        if (ImGui::CollapsingHeader(this == cameraNode.get()?"Camera": name.c_str())){
            ImGui::DragFloat3("Local Position",&position.x);
            ImGui::DragFloat3("Local Rotation",&rotationEuler.x);
            ImGui::DragFloat3("Local Scale",&scale.x);
            ImGui::Checkbox("Visualize skeleton",&visualizeSkeleton);
            auto globalPos = localToWorld() * glm::vec4(0,0,0,1); // transform 0,0,0 (pivot point) from local coordinate frame to global coordinate frame
            bool changed = ImGui::DragFloat3("Global Position",&globalPos.x);
            if (changed){
                if (parent){
                    position = glm::inverse(parent->localToWorld()) * globalPos;
                }
                else {
                    position = globalPos;
                }
            }
            if (ImGui::Button("Add child")){
                children.push_back(std::make_shared<Node>(this));
            }
            if (ImGui::Button("Make camera")){
                cameraNode = this->shared_from_this();
            }
            for (auto & n : children){
                n->gui(level+1);
            }
        }
        ImGui::Unindent(level*20);
        ImGui::PopID();
    }
};

class AssImp {
public:
    void ImportModel(const std::string& filename, Node* root){
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(filename.c_str(), aiProcess_Triangulate );

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
            return;
        }

        this->directory = filename.substr(0, filename.find_last_of('/'));
        this->processMaterials(scene);
        this->processMeshes(scene);
        this->processAnimations(scene);
        this->processNode(scene->mRootNode, scene, root);
    }
private:
    std::string directory;
    std::vector<std::shared_ptr<sre::Mesh>> meshes;
    std::vector<std::shared_ptr<sre::Material>> materials;

    void processAnimations(const aiScene *scene){
        std::cout << "Animations "<<scene->mNumAnimations<<endl;
        for (int i=0;i<scene->mNumAnimations;i++){
            std::cout << "  Animation "<<scene->mAnimations[i]->mName.C_Str()<<" Channels "<<scene->mAnimations[i]->mNumChannels<<endl;
            for (int j=0;j<scene->mAnimations[i]->mNumChannels;j++){
                std::cout << "    "<<scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str()<<" "<<
                        scene->mAnimations[i]->mChannels[j]->mNumPositionKeys<<" "<<
                        scene->mAnimations[i]->mChannels[j]->mNumRotationKeys<<" "<<
                        scene->mAnimations[i]->mChannels[j]->mNumScalingKeys<<endl;
            }
        }
    }

    void processMeshes(const aiScene *scene){
        for (int i=0;i<scene->mNumMeshes;i++){
            auto importedMesh = this->processMesh(scene->mMeshes[i]);
            meshes.push_back(importedMesh);
        }
    }

    std::shared_ptr<sre::Material> processMaterial(aiMaterial *pMaterial) {
        auto material = sre::Shader::getStandardPhong()->createMaterial();

        std::cout << "material "<<  pMaterial->mNumProperties<<std::endl;
        for (int i=0;i<pMaterial->mNumProperties;i++){
            auto prop = pMaterial->mProperties[i];
            if (prop->mKey == aiString("NAME")){
                material->setName(prop->mData);
            } else if (prop->mKey == aiString("COLOR_DIFFUSE")){
                aiColor3D color;
                pMaterial->Get(prop->mKey.C_Str(),prop->mType,prop->mIndex, color);
                material->setColor({color.r,color.g,color.b});
            } else if (prop->mKey == aiString("aiTextureType_DIFFUSE")){

            }

            std::cout << pMaterial->mProperties[i]->mKey.C_Str()<<std::endl;
        }
        return material;
    }

    void processMaterials(const aiScene *scene){
        for (int i=0;i<scene->mNumMaterials;i++){
            auto mat = this->processMaterial(scene->mMaterials[i]);
            materials.push_back(mat);
        }
    }

    void processNode(aiNode *node, const aiScene *scene, Node *parent) {
        auto child = std::make_shared<Node>(parent);
        child->name = node->mName.C_Str();
        parent->children.push_back(child);
        aiVector3t<float> scaling;
        aiVector3t<float> rotation;
        aiVector3t<float> position;
        node->mTransformation.Decompose(scaling,rotation, position);
        child->position = {position.x,position.y,position.z};
        child->rotationEuler = glm::degrees(glm::vec3{rotation.x,rotation.y,rotation.z});
        child->scale = {scaling.x,scaling.y,scaling.z};
        // Process all the node's meshes (if any)
        for(int i = 0; i < node->mNumMeshes; i++)
        {
            auto mesh = this->meshes[node->mMeshes[i]];
            child->meshes.push_back(mesh);
            auto material = this->materials[0];
            child->materials.push_back(material);
        }

        // Then do the same for each of its children
        for(int i = 0; i < node->mNumChildren; i++)
        {
            this->processNode(node->mChildren[i], scene, child.get());
        }
    }

    std::shared_ptr<sre::Mesh> processMesh(aiMesh *mesh){
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec4> colors;
        std::vector<glm::vec4> uvs;
        std::vector<uint32_t> indices;
        std::vector<int> boneCount(mesh->mNumVertices, 0);
        std::vector<glm::vec4>  animationBoneWeights(mesh->mNumVertices, glm::vec4(0));
        std::vector<glm::i32vec4> animationBoneIndices(mesh->mNumVertices, glm::i32vec4(0));

        for(int i = 0; i < mesh->mNumVertices; i++)
        {
            positions.emplace_back(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z);

            if (mesh->HasNormals()){
                normals.emplace_back(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z);
            }
            if (mesh->HasTextureCoords(0) && mesh->HasTextureCoords(1)){
                uvs.emplace_back(mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y,mesh->mTextureCoords[1][i].x,mesh->mTextureCoords[1][i].y);
            } else if (mesh->HasTextureCoords(0)){
                uvs.emplace_back(mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y,0,0);
            }
            if (mesh->HasVertexColors(0)){
                colors.emplace_back(mesh->mColors[0][i].r,mesh->mColors[0][i].g,mesh->mColors[0][i].b,mesh->mColors[0][i].a);
            }
        }

        if (mesh->HasFaces()){
            // always triangles (due to triangulation)
            for(int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for(int j = 0; j < face.mNumIndices; j++){
                    indices.push_back((uint32_t)face.mIndices[j]);
                }
            }
        }
        int maxBoneCount = 0;
        if (mesh->HasBones()){
            for (int i=0;i<mesh->mNumBones;i++){
                for (int j = 0;j<mesh->mBones[i]->mNumWeights;j++){
                    int vertexId = mesh->mBones[i]->mWeights[j].mVertexId;
                    float weight = mesh->mBones[i]->mWeights[j].mWeight;
                    int boneId = boneCount[vertexId];
                    if (weight==0) continue;
                    if(boneId<4){
                        animationBoneWeights[vertexId][boneId] = weight;
                        animationBoneIndices[vertexId][boneId] = vertexId;
                    } else {
                        int idWithMinWeight = 0;
                        float minWeight = animationBoneIndices[vertexId][0];
                        for (int i=1;i<4;i++){
                            if (animationBoneIndices[vertexId][i]<minWeight){
                                minWeight = animationBoneIndices[vertexId][i];
                                idWithMinWeight = i;
                            }
                        }
                        animationBoneWeights[vertexId][idWithMinWeight] = weight;
                        animationBoneIndices[vertexId][idWithMinWeight] = vertexId;
                    }

                    boneCount[vertexId]++;
                    maxBoneCount = std::max(maxBoneCount,boneCount[vertexId]);
                }
            }
            // normalize bones
            for (int i=0;i<animationBoneWeights.size();i++){
                float sum = 0;
                for (int j=0;j<4;j++){
                    sum += animationBoneWeights[i][j];
                }
                for (int j=0;j<4;j++){
                    animationBoneWeights[i][j] = animationBoneWeights[i][j] / sum;
                }
            }
            cout << "maxBoneCount " <<maxBoneCount<<endl;
        }


        if (mesh->HasBones()){
            return sre::Mesh::create()
                    .withName(mesh->mName.C_Str())
                    .withPositions(positions)
                    .withNormals(normals)
                    .withUVs(uvs)
                    .withColors(colors)
                    .withAttribute("boneWeight",animationBoneWeights)
                    .withAttribute("boneIndex",animationBoneIndices)
                    .withIndices(indices)
                    .build();
        }
        return sre::Mesh::create()
                .withName(mesh->mName.C_Str())
                .withPositions(positions)
                .withNormals(normals)
                .withUVs(uvs)
                .withColors(colors)
                .withIndices(indices)
                .build();
    }
};



class AssImpExample {
public:

    void setTexture(Node* node, std::shared_ptr<Texture> tex){
        for (auto& mat:node->materials){
            mat->setTexture(tex);
        }
        for (auto& n:node->children){
            setTexture(n.get(), tex);
        }
    }
    AssImpExample(std::string modelFile,std::string textureFile){
        r.init();

        camera.setPerspectiveProjection(60,0.1,1000);

        material = Shader::getStandardBlinnPhong()->createMaterial();
        material->setColor({1.0f,1.0f,1.0f,1.0f});
        material->setSpecularity({0.5,0.5,0.5,20.0f});

        AssImp imp;
        imp.ImportModel(modelFile, &root);

        if (textureFile.length()>0) {
            auto tex = Texture::create().withFile(textureFile).build();
            setTexture(&root, tex);
        }

        worldLights.setAmbientLight({0.2,0.2,0.2});
        worldLights.addLight(Light::create().withDirectionalLight(glm::normalize(glm::vec3{1, -1,1})).withColor({1,1,1}).build());

        root.children.push_back(std::make_shared<Node>(nullptr));

        cameraNode = root.children[1];
        cameraNode->position = {30,0,90};

        r.frameRender = [&](){
            auto localToWorld = cameraNode->localToWorld();
            camera.setViewTransform(glm::inverse(localToWorld));
            render();
        };

        r.startEventLoop();
    }

    void visualizeSkeleton(std::shared_ptr<Node>& node, glm::vec4 position, RenderPass& rp)
    {

        glm::vec4 localSpacePoint(0,0,0,1);
        glm::vec4 worldSpacePoint = node->localToWorld() * localSpacePoint;

        rp.drawLines({glm::vec3(position),glm::vec3(worldSpacePoint) });

        // draw children
        for (auto & n : node->children){
            visualizeSkeleton(n, worldSpacePoint, rp);
        }
    }

    void render(std::shared_ptr<Node>& node, RenderPass& rp){
        if (node->visualizeSkeleton){
            glm::vec4 localSpacePoint(0,0,0,1);
            glm::vec4 worldSpacePoint = node->localToWorld() * localSpacePoint;
            // draw children
            for (auto & n : node->children){
                visualizeSkeleton(n, worldSpacePoint, rp);
            }
        } else {
            for (int i=0;i<node->meshes.size();i++){
                rp.draw(node->meshes[i], node->localToWorld(), node->materials[i]);
            }

            // draw children
            for (auto & n : node->children){
                render(n, rp);
            }
        }
    }

    void render(){
        auto renderPass = RenderPass::create()
                .withCamera(camera)
                .withWorldLights(&worldLights)
                .withClearColor(true, {1, 0, 0, 1})
                .build();

        for (auto & n : root.children){
            render(n, renderPass);
        }

        // draw gui
        ImGui::Begin("SceneGraph");
        if (ImGui::Button("Add object")){
            root.children.push_back(std::make_shared<Node>(nullptr));
        }
        for (auto & n : root.children){
            n->gui();
        }
        ImGui::End();

        static Inspector inspector;
        inspector.update();
        inspector.gui();

        i++;
    }
private:
    SDLRenderer r;
    Camera camera;
    WorldLights worldLights;
    std::shared_ptr<Material> material;
    Node root;
    int i=0;
};


int main(int argc, const char **argv){
    std::string model;
    std::string texture;
    if (argc<=1){
        cout << "No file specified"<<endl;
        return -1;
    }
    model = argv[1];
    if (argc>=3){
        texture = argv[2];
    }
    cout << "Loading "<<model<<endl;
    new AssImpExample(model, texture);
    return 0;
}
