#include <unistd.h> // sleep()
#include "FourthRiver.h"
#include "Object.h"
#include "Components.h"

/* 
 * In the near future it shall load a gropu from a file and display it, allowing you to
 * modify the groups transform and other jazz so that you can see its usefullness
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
    Group mCubeGroup;

public:
    void printBuf(JGL::GPUBuffer& buf, bool tf)
    {
        buf.GetAccess( GL_READ_ONLY );
        for ( size_t i = 0; i < buf.count(); i += tf ? 3 : 1 )
        {
            std::cout << ((JM::Vect3*)buf.data())[i] << ", \n";
            if (tf)
            {
                std::cout << "uv: "<< ((JM::Vect3*)buf.data())[i+1] << ", \n";
                std::cout << "nr: "<< ((JM::Vect3*)buf.data())[i+2] << ", \n";

            }
        }

        buf.ReleaseAccess();
    }

    void OnLoad() override
    {  
        mCubeGroup.LoadGroupFromObjectFile<Cube>( "assets/groupTest.obj", 0 ); 

        mCubeGroup.Render( this );

        int i = 0;
        for ( const auto& ptr : mCubeGroup.mObjects )
        {
            const auto& o = *ptr;
            const auto& mesh = o.mesh;

            std::cout << "\n\n---------- "<< i++ << " ---------";
            std::cout << "\n-VBO-\n";
            printBuf( mesh->VBO, true );
            std::cout << "\n-IBO-\n";
            printBuf( mesh->IBO, false );
        }
    }

    void OnExit() override
    {  }

    void OnUpdate() override
    {
        if ( GetContext().GetKey( GLFW_KEY_ESCAPE ) == GLFW_PRESS )
            Exit();

        mCubeGroup.Render( this );

        // ImGui window
        {
            float trash;
            ImGui::Begin("DEBUG WINDOW!");
            ImGui::Text("%.2fms (%i FPS)", GetContext().FrameTime, GetContext().FPS );
            ImGui::SliderFloat( "scale", &trash, 0.0f, 0.1f  );
            ImGui::Text("%i DrawCalls", GetContext().DrawCalls );
            ImGui::End();
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



