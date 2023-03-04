#include <unistd.h> // sleep()
#include "TigerEngine.h"
#include "Object.h"
#include "Components.h"

/* 
 * Loads a kitten from a file and dsiplays it
 *
 * */

/*
class CubeGroup :
    public TigerEngine::Components::Group<Cube>
    public TigerEngine::Components::Transform
{
    CubeGroup()
    {
        Components::Group<Cube>::LoadGroupFromFile( "assets/chair.obj",  );

    }

}
*/

class Kitten : 
    public TigerEngine::Components::Transform,
    public TigerEngine::Components::Shader,
    public TigerEngine::Components::Mesh
{
public:


public:
    Cube()
    {
        // TODO: Components::Mesh::LoadFromFile( "assets/cube.obj", 0 );
        // VertexAttribute va = 
        //      Components::Mesh::LoadFromFile( "assets/chair.obj", 0, 4, &genChairLightInfo )
        // A object ends when you encounter a 'o' or a 'g'
        // if no o or g exist, and faces are defined, then it's an unnamed obj

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

class Test1 : public JGL::Scene
{
public:
    Test1() : 
        Scene( TigerEngine::GetRenderContext() )
    {
    }

public:
    /* Objects */
    Cube cube;

public:
    void OnLoad() override
    {
        /* Sets mouse to be invisible and its callback */
        // glfwSetInputMode( GetContext().renderContext().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
        // mCallbackManager.PerspectiveCameraMouseController();
    }

    void OnExit() override
    {

    }

    void OnUpdate() override
    {
        if ( GetContext().GetKey( GLFW_KEY_ESCAPE ) == GLFW_PRESS )
            Exit();

        if ( GetContext().GetKey( GLFW_KEY_TAB ) == GLFW_PRESS )
            Pause();

        cube.Render( this );

        // ImGui window
        {
            ImGui::Begin("DEBUG WINDOW!");
            ImGui::Text("%.2fms (%i FPS)", GetContext().FrameTime, GetContext().FPS );
            ImGui::Text("%.2f yaw %.2f pitch", GetCamera().mYaw, GetCamera().mPitch);

            ImGui::Text("%i DrawCalls", GetContext().DrawCalls );
            ImGui::End();
        }

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


void TigerEngine::OnLoad()
{

}


void TigerEngine::Main()
{

    std::cout << "-- SCENE 1 --" << std::endl;
    TigerEngine::SceneData scene = TigerEngine::LoadScene<Test1>();

    std::cout << "-- SCENE 2 --" << std::endl;
    TigerEngine::LoadScene<Test1>();

    if ( scene.scene ) /* Unpause scene 1 if it was paused */
    {
        std::cout << "-- SCENE 1 --" << std::endl;
        scene.scene->Unpause();
    }
}

void TigerEngine::OnExit() 
{

} 

int main()
{

    TigerEngine::Initialize( " Test ", 500, 500 );
    TigerEngine::Terminate();
    return 0;
}



