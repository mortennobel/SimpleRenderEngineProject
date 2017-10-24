#include "../Framework/Test.h"

#include "AddPair.h"
#include "ApplyForce.h"
#include "BasicSliderCrank.h"
#include "BodyTypes.h"
#include "Breakable.h"
#include "Bridge.h"
#include "BulletTest.h"
#include "Cantilever.h"
#include "Car.h"
#include "ContinuousTest.h"
#include "Chain.h"
#include "CharacterCollision.h"
#include "CollisionFiltering.h"
#include "CollisionProcessing.h"
#include "CompoundShapes.h"
#include "Confined.h"
#include "ConvexHull.h"
#include "ConveyorBelt.h"
#include "DistanceTest.h"
#include "Dominos.h"
#include "DumpShell.h"
#include "DynamicTreeTest.h"
#include "EdgeShapes.h"
#include "EdgeTest.h"
#include "Gears.h"
#include "HeavyOnLight.h"
#include "HeavyOnLightTwo.h"
#include "Mobile.h"
#include "MobileBalanced.h"
#include "MotorJoint.h"
#include "OneSidedPlatform.h"
#include "Pinball.h"
#include "PolyCollision.h"
#include "PolyShapes.h"
#include "Prismatic.h"
#include "Pulleys.h"
#include "Pyramid.h"
#include "RayCast.h"
#include "Revolute.h"
#include "RopeJoint.h"
#include "SensorTest.h"
#include "ShapeEditing.h"
#include "SliderCrank.h"
#include "SphereStack.h"
#include "TheoJansen.h"
#include "Tiles.h"
#include "TimeOfImpact.h"
#include "Tumbler.h"
#include "VaryingFriction.h"
#include "VaryingRestitution.h"
#include "VerticalStack.h"
#include "Web.h"

#include <string>
#ifndef FOOTEST_H
#define FOOTEST_H

#include "../Framework/Test.h"

class FooTest : public Test
{
public:
    FooTest() {
        m_button = true;
        
        // create border
        {
            b2BodyDef bd;
            b2Body* ground = m_world->CreateBody(&bd);
            
            b2EdgeShape shape;
            shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
            ground->CreateFixture(&shape, 0.0f);
            
            shape.Set(b2Vec2(-40.0f, 20.0f), b2Vec2(40.0f, 20.0f));
            ground->CreateFixture(&shape, 0.0f);
            
            // create boxes
            b2PolygonShape box;
            box.SetAsBox(3, 0.5, b2Vec2(1.0f, 3.0f), 0.0f);
            ground->CreateFixture(&box, 0.0f);

        }
        
        b2CircleShape shape;
        shape.m_radius = 1.0f;
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.position.Set(0.0, 4.0f + 3.0f);
        
        m_bodies = m_world->CreateBody(&bd);
        
        m_bodies->CreateFixture(&shape, 1.0f);
        m_bodies->SetLinearVelocity(b2Vec2(3.0f, 0.0f));
        
        
    }
    
    void Step(Settings* settings)
    {
        //run the default physics and rendering
        Test::Step(settings);
        
        //show some text in the main screen
        std::string debugStr = "Collisions ";
        debugStr += std::to_string(m_pointCount);
        g_debugDraw.DrawString(5, m_textLine, debugStr.c_str());
        m_textLine += 15;
    }
    
    static Test* Create()
    {
        return new FooTest;
    }
    
    void Keyboard(int key)
    {
        switch (key)
        {
            case SDLK_a:
                m_button = true;
                break;
        }
    }
    
    void KeyboardUp(int key)
    {
        switch (key)
        {
            case SDLK_a:
                m_button = false;
                
                m_bodies->ApplyForce(b2Vec2(0.0f, 999.1f), m_bodies->GetPosition(), true);
                break;
        }
    }
    
    b2Body* m_bodies;
    
    bool m_button;
};

#endif
