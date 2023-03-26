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

public:

    void OnLoad() override
    {  
        auto data = mGroup.ParseGroupFile( "assets/groupTest.grp" );
    }

    void OnExit() override
    {  }

    void OnUpdate() override
    {
        if ( GetContext().GetKey( GLFW_KEY_ESCAPE ) == GLFW_PRESS )
            Exit();
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



