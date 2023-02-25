#include <unistd.h> // sleep()
#include "TigerEngine.h"
#include "Object.h"
#include "Components.h"

/* 
 * Test1 should be just a basic test to see if
 * everything is working correctly
 * */


class Rectangle : 
    public TigerEngine::Components::Transform,
    public TigerEngine::Components::Shader
{
public:

};

class Test1 : public JGL::Scene
{
public:
    Test1() : 
        Scene( TigerEngine::GetRenderContext() )
    {
    }

public:
    void OnLoad() override
    {
        /* Sets mouse to be invisible and its callback */
        glfwSetInputMode( GetContext().renderContext().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
        // mCallbackManager.PerspectiveCameraMouseController();

        /* define rectangle data */
        float modelData[] = 
        {   /* 3D pos */        /* Color       */    /* UV   */
            -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
             0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
             0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,

            -0.5f, -0.5f, -1.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
             0.5f, -0.5f, -1.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
             0.5f,  0.5f, -1.0f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f, -1.0f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f
        };
        uint32_t modelIndices[] = // Counter clockwise winding
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

        /* Set rectangle data */
        rectMesh.VBO.Alloc( sizeof(modelData), GL_ARRAY_BUFFER, 
                            GL_STATIC_DRAW, modelData, sizeof( modelData ) / sizeof( modelData[0] ) );

        rectMesh.IBO.Alloc( sizeof( modelIndices), GL_ELEMENT_ARRAY_BUFFER, 
                            GL_STATIC_DRAW, modelIndices,  sizeof( modelIndices ) / sizeof( modelIndices[0] ) );

        JGL::VertexAttribute va;
            va.Add( GL_FLOAT, 3 );
            va.Add( GL_FLOAT, 3 );
            va.Add( GL_FLOAT, 2 );
        rectMesh.VAO.AddAttrib( rectMesh.VBO, va );

        rectMesh.Primitive = GL_TRIANGLES;

        rect.mesh = &rectMesh;
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

        rect.Render( this );

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

private:
    JGL::Mesh rectMesh;
    Rectangle rect;

    uint32_t uColor = 0;
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

    std::cout << "-- SCENE 3 --" << std::endl;
    TigerEngine::LoadScene<JGL::Scene>( TigerEngine::GetRenderContext() );
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



