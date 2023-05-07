#include <unistd.h> // sleep()
#include "FourthRiver.h"
#include "Object.h"
#include "Components.h"
#include "src/ComponentManagerTemplates.cpp"

/* 
 * Test case for lighting
 *
 * */

class Cube 
    : public FourthRiver::Components::Transform,
      public FourthRiver::Components::Mesh,
      public FourthRiver::Components::Shader
{  
public:
    Cube()
    {
        Mesh::LoadSimpleFromFile( "assets/cube.obj", 0);
    }
};

class LightCube
    : public FourthRiver::Components::Transform,
      public FourthRiver::Components::Mesh,
      public FourthRiver::Components::LightSource
{
public:
    LightCube() : LightSource( { 1.0f, 0.0f, 0.0f } )
    {  
        Mesh::LoadSimpleFromFile( "assets/cube.obj", 0 );
    }
};


class LightingTest : public JGL::Scene
{
public:
    LightingTest() : Scene( FourthRiver::GetRenderContext() )
    {  }

public:
    Cube cube;
    LightCube light;

public:

    void OnLoad() override
    {  
        light.Position = { 2.0f, 1.0f, 0.5f };
    }

    void OnExit() override
    {  }

    void OnUpdate() override
    {
        if ( GetContext().GetKey( GLFW_KEY_TAB ) == GLFW_PRESS )
            Exit();

        cube.Render( this );
        light.Render( this );

        {
            ImGui::Begin("DEBUG WINDOW!");
            ImGui::Text("%.2fms (%i FPS)\n", GetContext().FrameTime, GetContext().FPS );
            ImGui::Text("\n%i DrawCalls", GetContext().DrawCalls );
            ImGui::End();
        }
    }

    void OnFixedUpdate( float dt ) override
    {
        static size_t p = 0;

        DefaultKeyboardCameraMove( dt, 0.001f, 100.0f );
        light.UpdateInformation( this );

        if ( p++ * dt >= 1000 )
        {
            mLightManager.UpdateBuffer();
            p = 0;
        }
    }
};


void FourthRiver::OnLoad()
{  }


void FourthRiver::Main()
{
    FourthRiver::LoadScene<LightingTest>();
}

void FourthRiver::OnExit() 
{

} 

int main()
{
    FourthRiver::Initialize( " Test ", 500, 500 );
    FourthRiver::Terminate();
}



