#include <unistd.h> // sleep()
#include "FourthRiver.h"
#include "Object.h"
#include "Components.h"
#include "src/ComponentManagerTemplates.cpp"

/* 
 * Loads 2 groups from a .obj file and allows you to modify transform
 *
 * */

class Cube 
    : public FourthRiver::Components::Transform,
      public FourthRiver::Components::Mesh,
      public FourthRiver::Components::Shader
{  };

class Group
    : public FourthRiver::ObjectGroup,
      public FourthRiver::Components::Transform
{  };


class GroupTest : public JGL::Scene
{
public:
    GroupTest() : Scene( FourthRiver::GetRenderContext() )
    {  }

public:
    /* Objects */
    Group mCubeGroup1;
    Group mCubeGroup2;

public:

    void OnLoad() override
    {  
        mCubeGroup1.LoadGroupFromObjectFile<Cube>( "assets/groupTest.obj", 0 ); 
        mCubeGroup2.LoadGroupFromObjectFile<Cube>( "assets/groupTest.obj", 1 ); 

        std::cout << mCubeGroup1.name << std::endl;
        std::cout << mCubeGroup2.name << std::endl;
    }

    void OnExit() override
    {  }

    void OnUpdate() override
    {
        if ( GetContext().GetKey( GLFW_KEY_ESCAPE ) == GLFW_PRESS )
            Exit();

        mCubeGroup1.Render( this );
        mCubeGroup2.Render( this );

        // ImGui window
        {
            float scale1 = mCubeGroup1.Scale.x();
            float scale2 = mCubeGroup2.Scale.x();

            ImGui::Begin("DEBUG WINDOW!");
            ImGui::Text("%.2fms (%i FPS)", GetContext().FrameTime, GetContext().FPS );
            ImGui::SliderFloat( "G1_Scale", &scale1, 0.0f, 1.0f  );
            ImGui::SliderFloat3( "G1_Pos", &mCubeGroup1.Position.x(), -10.0f, 10.0f  );

            ImGui::SliderFloat( "G2_Scale", &scale2, 0.0f, 1.0f  );
            ImGui::SliderFloat3( "G2_Pos", &mCubeGroup2.Position.x(), -10.0f, 10.0f  );

            ImGui::SliderFloat3( "o1_Pos", &mCubeGroup1.GetObjectAs<Cube>(0).Position.x(), -10.0f, 10.0f  );

            ImGui::Text("%i DrawCalls", GetContext().DrawCalls );
            ImGui::End();

            mCubeGroup1.Scale = { scale1 };
            mCubeGroup2.Scale = { scale2 };
        }
    }

    void OnFixedUpdate( float dt ) override
    {
        DefaultKeyboardCameraMove( dt, 0.001f, 100.0f );
    }
};


void FourthRiver::OnLoad()
{  }


void FourthRiver::Main()
{
    FourthRiver::LoadScene<GroupTest>();
}

void FourthRiver::OnExit() 
{

} 

int main()
{
    FourthRiver::Initialize( " Test ", 500, 500 );
    FourthRiver::Terminate();
}



