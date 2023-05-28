#include "FourthRiver.h"
#include "Components.h"


class Cube
    : public FourthRiver::Components::Mesh,
      public FourthRiver::Components::Shader,
      public FourthRiver::Components::Transform
{
public:
    Cube()
    {
        LoadSimpleFromFile( "assets/cube.obj", 0 );
    }
};

class Simple 
    : public JGL::Scene
{
public:
    Simple() : Scene( FourthRiver::GetRenderContext() )
    {  }

public:
    /* Objects */
    Cube cube;

public:

    void OnLoad() override
    {  }

    void OnExit() override
    {  }


    void OnUpdate() override
    {
        if ( GetContext().GetKey( GLFW_KEY_TAB ) == GLFW_PRESS )
            Exit();

        /* Render Objects */
        cube.Render( this );

        /* ImGUI */
        ImGui::Begin("DEBUG WINDOW!");
        ImGui::Text("%.2fms (%i FPS)\n", GetContext().FrameTime, GetContext().FPS );

        ImGui::Text("VERTEX SHADER: \n%s\nFRAG SHADER\n%s", 
                cube.V_SourceComplete().c_str(),
                cube.F_SourceComplete().c_str() );

        ImGui::Text("\n%i DrawCalls", GetContext().DrawCalls );
        ImGui::End();
    }

    void OnFixedUpdate( float dt ) override
    {
        DefaultKeyboardCameraMove( dt, 0.001f, 100.0f );
    }
};



void FourthRiver::OnLoad() {  }
void FourthRiver::OnExit() {  }

void FourthRiver::Main() { FourthRiver::LoadScene<Simple>(); }

int main()
{
    FourthRiver::Initialize( "Simple Demo", 1024, 956 );
    FourthRiver::Terminate();
}
