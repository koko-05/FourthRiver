#include <unistd.h> // sleep()
#include "FourthRiver.h"
#include "Object.h"
#include "Components.h"

/* 
 * Loads a kitten and a cube from a file and dsiplays it
 *
 * */

class Kitten : 
    public FourthRiver::Components::Transform,
    public FourthRiver::Components::Shader,
    public FourthRiver::Components::Mesh
{
public:


public:
    Kitten( size_t index = 0 )
    {
        size_t dataSize, dataESize;
        std::vector<Mesh::FileData> fileData;
        std::vector<uint32_t>       fileIndices;

        Mesh::LoadFromFile( "assets/cat.obj", index, fileData, fileIndices );
        auto fileDataPtr = Mesh::FileData::GetDataFromVector(fileData, dataSize, dataESize);

        constexpr uint32_t u32s = sizeof(uint32_t);
        Mesh::CreateVertexBuffer( GL_STATIC_DRAW, 3, fileDataPtr.get(), dataSize, dataESize );
        Mesh::CreateIndexBuffer( GL_STATIC_DRAW, fileIndices.data(), fileIndices.size() * u32s, u32s );

        JGL::VertexAttribute attrib; 
        Mesh::CreateAttributeFromFileData( fileData, attrib );
        Mesh::VAO.AddAttrib( Mesh::VBO, Mesh::IBO, attrib );
    }

public:
    bool isVisible = true;

};


class Test1 : public JGL::Scene
{
public:
    Test1() : 
        Scene( FourthRiver::GetRenderContext() )
    {
    }

public:
    class ObjectClone
        : public FourthRiver::Components::Transform
    {  
    public:
        ObjectClone( const FourthRiver::Object& obj )
        {
            mesh   = obj.mesh;
            shader = obj.shader;
        }
    };

public:
    /* Objects */
    Kitten kitten;
    Kitten cube{1}; /* there's a hidden cube object in the cat.obj file */

public:
    void OnLoad() override
    {  
        //cube.Position = { 0, -10, 0 };
    }

    void OnExit() override
    {  }

    void OnUpdate() override
    {
        static float scaleVal = 0.01f;
        kitten.Scale = { scaleVal };

        if ( GetContext().GetKey( GLFW_KEY_ESCAPE ) == GLFW_PRESS )
            Exit();

        if ( kitten.isVisible )
            kitten.Render( this );

        cube.Render( this );

        // ImGui window
        {
            ImGui::Begin("DEBUG WINDOW!");
            ImGui::Text("%.2fms (%i FPS)", GetContext().FrameTime, GetContext().FPS );
            ImGui::SliderFloat( "scale", &scaleVal, 0.0f, 0.1f  );
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


void FourthRiver::OnLoad()
{

}


void FourthRiver::Main()
{
    FourthRiver::LoadScene<Test1>();
}

void FourthRiver::OnExit() 
{

} 

int main()
{

    FourthRiver::Initialize( " Test ", 500, 500 );
    FourthRiver::Terminate();
    return 0;
}



