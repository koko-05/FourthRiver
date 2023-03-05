#include <unistd.h> // sleep()
#include "TigerEngine.h"
#include "Object.h"
#include "Components.h"

/* 
 * Loads a kitten from a file and dsiplays it
 *
 * */

class Kitten : 
    public TigerEngine::Components::Transform,
    public TigerEngine::Components::Shader,
    public TigerEngine::Components::Mesh
{
public:


public:
    Kitten()
    {
        std::vector<Mesh::FileData> fileData;
        std::vector<uint32_t>       fileIndices;
        Mesh::LoadFromFile( "assets/kitten.obj", 0, fileData, fileIndices );
        size_t dataSize;
        size_t dataESize;
        auto fileDataPtr = Mesh::FileData::GetDataFromVector(fileData, dataSize, dataESize);
        Mesh::CreateVertexBuffer( GL_STATIC_DRAW, 3, fileDataPtr.get(), dataSize, dataESize );
        Mesh::CreateIndexBuffer( GL_STATIC_DRAW, fileIndices.data(), fileIndices.size() * sizeof( uint32_t ), sizeof( uint32_t ) );
        JGL::VertexAttribute attrib;
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
    Kitten kitten;

public:
    void OnLoad() override
    {  }

    void OnExit() override
    {  }

    void OnUpdate() override
    {
        if ( GetContext().GetKey( GLFW_KEY_ESCAPE ) == GLFW_PRESS )
            Exit();

        if ( GetContext().GetKey( GLFW_KEY_TAB ) == GLFW_PRESS )
            Pause();

        kitten.Render( this );

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



