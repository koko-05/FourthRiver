#include <unistd.h> // sleep()
#include "FourthRiver.h"
#include "Object.h"
#include "Components.h"

/* 
 * Basic cube example, you can move the camera with the arrows and w,s,a,d
 *
 * */

class Cube : 
    public FourthRiver::Components::Transform,
    public FourthRiver::Components::Shader,
    public FourthRiver::Components::Mesh
{
public:
    Cube()
    {
        const std::array<float, 64> modelData = 
        {   
            /* 3D pos */         /* Color      */    /* UV   */
            -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
             0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
             0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,

            -0.5f, -0.5f, -1.0f, 1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
             0.5f, -0.5f, -1.0f, 0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
             0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f, -1.0f, 0.5f, 0.5f, 0.5f,   0.0f, 1.0f
        };

        const std::array<uint16_t, 36> cubeIndices =
        {
            /* front face */
            0, 1, 2,
            2, 3, 0,
            /* back face */
            4, 5, 6,
            6, 7, 4,
            /* right face */
            1, 5, 6,
            6, 2, 1,
            /* left face */
            4, 0, 3,
            3, 7, 4,
            /* top face */
            3, 2, 6,
            6, 7, 3,
            /* bottom face */
            1, 0, 4,
            4, 5, 1,
        };

        Mesh::LoadFromData( modelData, cubeIndices, GL_STATIC_DRAW );
        JGL::VertexAttribute attrib;
                             attrib.Add( GL_FLOAT, 3 );
                             attrib.Add( GL_FLOAT, 3 );
                             attrib.Add( GL_FLOAT, 2 );
        Mesh::VAO.AddAttrib( Mesh::VBO, attrib );
    }

};

class CubeScene : public JGL::Scene
{
public:
    CubeScene() : 
        Scene( FourthRiver::GetRenderContext() )
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
        if ( GetContext().GetKey( GLFW_KEY_ESCAPE ) == GLFW_PRESS )
            Exit();

        cube.Render( this );
    }

    void OnFixedUpdate( float dt ) override
    {
        /* Camera Controll */
        auto& cam = GetCamera();
        const float speed = 0.001f * dt;

        const JM::Vect3 up       = { 0.0f,   speed,  0.0f };
        const JM::Vect3 down     = { 0.0f,   -speed, 0.0f};

        if ( GetContext().GetKey( GLFW_KEY_W ) == GLFW_PRESS )
            cam.Move( GetCamera().Direction * -speed );

        if ( GetContext().GetKey( GLFW_KEY_S ) == GLFW_PRESS )
            cam.Move( GetCamera().Direction *  speed );

        if ( GetContext().GetKey( GLFW_KEY_A ) == GLFW_PRESS )
            cam.Move( GetCamera().RightAxis * -speed );

        if ( GetContext().GetKey( GLFW_KEY_D ) == GLFW_PRESS )
            cam.Move( GetCamera().RightAxis *  speed );

        if ( GetContext().GetKey( GLFW_KEY_SPACE ) == GLFW_PRESS )
            cam.Move( up );

        if ( GetContext().GetKey( GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS )
            cam.Move( down );

        constexpr float camSpeed = 100.0f;
        if ( GetContext().GetKey( GLFW_KEY_LEFT ) == GLFW_PRESS )
            cam.MoveDirection( speed * -camSpeed, 0.0f );
        if ( GetContext().GetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS )
            cam.MoveDirection( speed *  camSpeed, 0.0f );
        if ( GetContext().GetKey( GLFW_KEY_UP ) == GLFW_PRESS )
            cam.MoveDirection( 0.0f, speed * -camSpeed );
        if ( GetContext().GetKey( GLFW_KEY_DOWN ) == GLFW_PRESS )
            cam.MoveDirection( 0.0f, speed * camSpeed );
    }
};

/* Tiger Engine */

void FourthRiver::OnLoad()
{  }


void FourthRiver::Main()
{
    FourthRiver::LoadScene<CubeScene>();
}

void FourthRiver::OnExit() 
{  } 

int main()
{

    FourthRiver::Initialize( " Test ", 500, 500 );
    FourthRiver::Terminate();
    return 0;
}



