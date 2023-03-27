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
    Group mGroup;
    FourthRiver::Components::Transform* SelectedTransform = nullptr;

public:

    void OnLoad() override
    {  
        mGroup.LoadFromGroupFile<Cube, Group>( "assets/groupTest.grp" );
    }

    void OnExit() override
    {  }

    void ImGuiRenderGroup( FourthRiver::ObjectGroup& grp, size_t depth = 0 )
    {
        static char stringBuffer[256] = { 0 };
        memset( stringBuffer, 0, 256 );

        size_t offset = 0;
        for ( ; offset < depth; offset++ )
            stringBuffer[offset] = '\t';

        memcpy( stringBuffer + offset, "GRP: ", 5 );
        memcpy( stringBuffer + offset + 5, grp.name, MAX_OSIZE );

        if ( ImGui::Button( stringBuffer ) )
            SelectedTransform = &( grp.GetComponent<FourthRiver::Components::Transform>() );

        stringBuffer[offset++] = '\t';
        for ( const auto& obj : grp.mObjects  )
        {
            memcpy( stringBuffer + offset, obj->name, MAX_OSIZE );
            if ( ImGui::Button( stringBuffer ) )
                SelectedTransform = &( obj->GetComponent<FourthRiver::Components::Transform>() );
        }

        for ( const auto& g : grp.mGroups )
            ImGuiRenderGroup( *g, depth + 1 );

    }

    void OnUpdate() override
    {
        if ( GetContext().GetKey( GLFW_KEY_ESCAPE ) == GLFW_PRESS )
            Exit();

        mGroup.Render( this );

        {
            ImGui::Begin("DEBUG WINDOW!");
            ImGui::Text("%.2fms (%i FPS)\n", GetContext().FrameTime, GetContext().FPS );

            if ( SelectedTransform )
            {
                float scale = SelectedTransform->Scale.x();

                ImGui::SliderFloat ( "Scale: ", &scale, 0.0f, 1.0f );
                ImGui::SliderFloat3( "Pos:   ", &SelectedTransform->Position.x(), -10.0f, 10.0f );
                ImGui::SliderFloat3( "Rot:   ", &SelectedTransform->Rotation.x(), -1.0f, 1.0f );

                SelectedTransform->Scale = { scale };
            }

            ImGuiRenderGroup( mGroup );

            ImGui::Text("\n%i DrawCalls", GetContext().DrawCalls );
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



