// Naive port of https://github.com/CreativeInquiry/ofxBussIK

#include <iostream>

#include "sre/Texture.hpp"
#include "sre/Renderer.hpp"
#include "sre/Material.hpp"
#include "LinearR3.h"
#include "Tree.h"
#include "Jacobian.h"

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sre/SDLRenderer.hpp>
#include <sre/impl/GL.hpp>
#include <sre/Inspector.hpp>
#include <glm/ext.hpp>

using namespace sre;

#define MAX_NUM_NODE	1000
#define MAX_NUM_THETA	1000
#define MAX_NUM_EFFECT	100
#define RADIAN(X)	((X)*RadiansToDegrees)

enum Method {
    IK_JACOB_TRANS=0,
    IK_PURE_PSEUDO,
    IK_DLS,
    IK_SDLS ,
    IK_DLS_SVD
};

// Make slowdown factor larger to make the simulation take larger, less frequent steps
// Make the constant factor in Tstep larger to make time pass more quickly
//const int SlowdownFactor = 40;
const int SlowdownFactor = 0;		// Make higher to take larger steps less frequently
const int SleepsPerStep=SlowdownFactor;
int SleepCounter=0;
//const double Tstep = 0.0005*(double)SlowdownFactor;		// Time step


int	AxesList;		/* list to hold the axes		*/
int	AxesOn;			/* ON or OFF				*/

float	Scale, Scale2;		/* scaling factors			*/



int JointLimitsOn;
int RestPositionOn;
int UseJacobianTargets1;


int numIteration = 1;
double error = 0.0;
double errorDLS = 0.0;
double errorSDLS = 0.0;
double sumError = 0.0;
double sumErrorDLS = 0.0;
double sumErrorSDLS = 0.0;

#ifdef _DYNAMIC
bool initMaxDist = true;
extern double Excess[];
extern double dsnorm[];
#endif

class BussIKApp{
public:
    BussIKApp(){
        r.setWindowSize(glm::ivec2(1024,768));
        r.init();



        mesh = Mesh::create()
                .withSphere()
                .build();

        worldLights.addLight(Light::create()
                                     .withDirectionalLight(glm::normalize(glm::vec3(1,1,1)))
                                     .build());

        mat1 = Shader::getStandardBlinnPhong()->createMaterial();
        mat1->setColor({1,1,1,1});
        mat1->setSpecularity(Color(1,1,1,50));

        mat2 = Shader::getUnlit()->createMaterial();
        mat2->setColor({1,1,0,1});
        mat2->setSpecularity(Color(0,0,0,0));

        r.frameUpdate = [&](float deltaTime){
            update(deltaTime);
        };
        r.frameRender = [&](){
            render();

        };

        BuildKukaIIWAShape();
        m_ikJacobian = new Jacobian(&m_ikTree);
        Reset(m_ikTree,m_ikJacobian);

        camera.setOrthographicProjection(2,0.001,10);
        camera.lookAt({0, 0, 1}, {0, 0, -1}, {0,1,0});
        //    cam.setFarClip(10);

        m_ikMethod = IK_JACOB_TRANS;

        std::cout << "d for debug" << std::endl;
        r.keyEvent = [&](SDL_Event& event){
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d){
                debug = true;
            }

        };
        r.startEventLoop();
    }

    void UpdateTargets( double T2, Tree & treeY) {
        double T3 = T2 / 5.;
        targetaa[0].Set(0.6*cos(T3), 0.9*sin(T3/2), 0.15);
        target = {0.6*cos(T3),  0.9*sin(T3/2), 0.15};
    }

    void Reset(Tree &tree, Jacobian* m_ikJacobian)
    {

        AxesOn = false;

        Scale  = 1.0;
        Scale2 = 0.0;		/* because add 1. to it in Display()	*/

        JointLimitsOn = true;
        RestPositionOn = false;
        UseJacobianTargets1 = false;


        tree.Init();
        tree.Compute();
        m_ikJacobian->Reset();

    }


    void BuildKukaIIWAShape(){
        const VectorR3& unitx = VectorR3::UnitX;
        const VectorR3& unity = VectorR3::UnitY;
        const VectorR3& unitz = VectorR3::UnitZ;
        const VectorR3 unit1(sqrt(14.0) / 8.0, 1.0 / 8.0, 7.0 / 8.0);
        const VectorR3& zero = VectorR3::Zero;

        float minTheta = -PI;
        float maxTheta = PI;

        m_ikNodes.resize(7);//6DOF+additional endeffector


        m_ikNodes[0] = new Node(VectorR3(0, 0, 0), unitz, 0.08, JOINT, -2*PI, 2*PI, glm::radians(0.));
        m_ikTree.InsertRoot(m_ikNodes[0]);

        m_ikNodes[1] = new Node(VectorR3(0, -0.072238, 0.083204), -unity, 0.08, JOINT, minTheta, maxTheta, glm::radians(-90.));
        m_ikTree.InsertLeftChild(m_ikNodes[0], m_ikNodes[1]);

        m_ikNodes[2] = new Node(VectorR3(0,-0.077537,0.51141), -unity, 0.08, JOINT, minTheta, maxTheta, glm::radians(0.));
        m_ikTree.InsertLeftChild(m_ikNodes[1], m_ikNodes[2]);

        m_ikNodes[3] = new Node(VectorR3(0, -0.070608, 0.903192), -unity, 0.08, JOINT, minTheta, maxTheta, glm::radians(-90.));
        m_ikTree.InsertLeftChild(m_ikNodes[2], m_ikNodes[3]);

        m_ikNodes[4] = new Node(VectorR3(0, -0.117242, 0.950973), unitz, 0.08, JOINT, minTheta, maxTheta, glm::radians(0.));
        m_ikTree.InsertLeftChild(m_ikNodes[3], m_ikNodes[4]);

        m_ikNodes[5] = new Node(VectorR3(0, -0.164751, 0.996802), unity, 0.08, JOINT, minTheta, maxTheta, glm::radians(0.));
        m_ikTree.InsertLeftChild(m_ikNodes[4], m_ikNodes[5]);

        m_ikNodes[6] = new Node(VectorR3(0, -0.164751, 0.996802), zero, 0.08, EFFECTOR, minTheta, maxTheta, glm::radians(0.));
        m_ikTree.InsertLeftChild(m_ikNodes[5], m_ikNodes[6]);
    }

    void update(float deltaTime){
        time += deltaTime;
        DoUpdateStep(deltaTime, m_ikTree, m_ikJacobian, m_ikMethod);
    }

    // Does a single update (on one kind of tree)
    void DoUpdateStep(double Tstep, Tree & treeY, Jacobian *jacob, int ikMethod) {
        if ( SleepCounter==0 ) {
            T += Tstep;
            UpdateTargets( T , treeY);
        }

        if ( UseJacobianTargets1 ) {
            jacob->SetJtargetActive();
        }
        else {
            jacob->SetJendActive();
        }
        jacob->ComputeJacobian(targetaa);						// Set up Jacobian and deltaS vectors

        // Calculate the change in theta values
        switch (ikMethod) {
            case IK_JACOB_TRANS:
                jacob->CalcDeltaThetasTranspose();		// Jacobian transpose method
                break;
            case IK_DLS:
                jacob->CalcDeltaThetasDLS();			// Damped least squares method
                break;
            case IK_DLS_SVD:
                jacob->CalcDeltaThetasDLSwithSVD();
                break;
            case IK_PURE_PSEUDO:
                jacob->CalcDeltaThetasPseudoinverse();	// Pure pseudoinverse method
                break;
            case IK_SDLS:
                jacob->CalcDeltaThetasSDLS();			// Selectively damped least squares method
                break;
            default:
                jacob->ZeroDeltaThetas();
                break;
        }

        if ( SleepCounter==0 ) {
            jacob->UpdateThetas();							// Apply the change in the theta values
            jacob->UpdatedSClampValue(targetaa);
            SleepCounter = SleepsPerStep;
        }
        else {
            SleepCounter--;
        }
    }

    void getNode(const Node* node, RenderPass& rp, glm::vec3& position, glm::quat& orientation)
    {
        glm::vec3 axis(node->v.x, node->v.y, node->v.z);
        std::cout << glm::to_string(axis)<<" "<<node->GetTheta() <<std::endl;
        orientation = glm::quat(1, 0, 0, 0);
        if (axis.length())
        {
            orientation = glm::angleAxis ((float)node->GetTheta(), axis);
        }

        //act.setPosition(node->s.x, node->s.y, node->s.z);
        position = {node->s.x, node->s.y, node->s.z};
        // draw rotation axis
        //ofDrawLine(act.getPosition(), act.getPosition() + axis);
        //rp.drawLines({position, position+axis});
    }

    void drawTree(Node* node, RenderPass& rp)
    {
        if (node != 0) {
            glm::vec3 position;
            glm::quat orientation;
            getNode(node, rp, position, orientation);

            drawNode(rp, position, orientation);

            if (node->left) {
                drawTree(node->left, rp);		// Draw tree of children recursively

                rp.drawLines({position, glm::vec3{node->left->s.x, node->left->s.y, node->left->s.z}});
            }
        }
    }

    void drawNode(sre::RenderPass & rp, glm::vec3 position,glm::quat orientation, bool alternativeAlt = false){
        float scale = 0.01;
        rp.draw(mesh, glm::translate(position)*glm::scale(glm::vec3{scale,scale,scale}), alternativeAlt?mat2:mat1);
    }

    void render(){
        auto rp = RenderPass::create()
                .withCamera(camera)
                .withWorldLights(&worldLights)
                .withClearColor(true,{1,0,0,1})
                .build();

        //ofNode act;
        glm::vec3 position;
        glm::quat orientation;
        getNode(m_ikTree.GetRoot(), rp, position, orientation);
        //ofSetColor(255, 255, 255);
        drawTree(m_ikTree.GetRoot(), rp);
        //ofSetColor(255, 0, 255);
        //target.draw();
        drawNode(rp, target, glm::quat(1,0,0,0), true);

        checkGLError();

        if (debug){
            static sre::Inspector inspector;
            inspector.update();
            inspector.gui();
        }

        checkGLError();

    }
private:
    float time;
    SDLRenderer r;
    Camera camera;
    std::shared_ptr<Mesh> mesh;
    WorldLights worldLights;
    std::shared_ptr<Material> mat1;
    std::shared_ptr<Material> mat2;
    bool debug = false;
    glm::mat4 pos1 = glm::translate(glm::mat4(1), {-1,0,0});
    glm::mat4 pos2 = glm::translate(glm::mat4(1), {1,0,0});

    double T = 0;
    VectorR3 targetaa[MAX_NUM_EFFECT];

    //ofEasyCam cam;

    glm::vec3 target;
    int m_ikMethod;
    Tree m_ikTree;
    vector<Node*> m_ikNodes;
    Jacobian* m_ikJacobian;

    vector<int> m_movingInstances;
    int m_targetInstance;
    enum
    {
        numCubesX = 20,
        numCubesY = 20
    };
};

int main() {
    new BussIKApp();

    return 0;
}

