#include "FourthRiver.h"
#include "JGL/Framebuffer.h"
#include "Components.h"
#include "src/ShaderPipelineModules.cpp"
#include "src/ComponentManagerTemplates.cpp"


class Cube
    : public FourthRiver::Components::Mesh,
      public FourthRiver::Components::Transform,
      public FourthRiver::Components::ShaderPipeline,
      public FourthRiver::Components::Texture
{
public:
    Cube()
        : mTexture( 2 )
    {
        mTexture.SetImgDataF( "assets/textures/test.png", GL_TEXTURE_2D, GL_RGB );
        SetTexture( mTexture );

        Cube_ShaderPipeline();
        LoadSimpleFromFile( "assets/cube.obj", 0 );

        ShaderPipeline::InitializeModules();
    }

    void Cube_ShaderPipeline()
    {
        FR_ShaderPipeline_Start;
        {
            AddModule<Core>();
            AddModule<MVP>();
            AddModule<Color>();
            
            AddModule( Texturer( "testTexture", u_samplers,
                "color *= texture ( testTexture, UV );"
            ));
        }
        FR_ShaderPipeline_End;
    }

    static void u_samplers( FourthRiver::Object* obj )
    {
        auto& sh = obj->GetComponent<FourthRiver::Components::Shader>();
        sh.SetUniform1i("testTexture", 0);
    }

public:
    JGL::Texture mTexture;
};


class FrameObject
    : public FourthRiver::Components::Mesh,
      public FourthRiver::Components::Texture,
      public FourthRiver::Components::Shader
{
public:
    FrameObject( JGL::Framebuffer& fb )
        : mTexture( fb.texture_color )
    {
        SetTexture( mTexture );
        SetShader( Vert, Frag );

        Mesh::LoadFromData( mesh_data_VBO, mesh_data_IBO, GL_STATIC_DRAW );

        JGL::VertexAttribute attrib;
                             attrib.Add( GL_FLOAT, 2, "pos" );
                             attrib.Add( GL_FLOAT, 2, "uv"  );
        Mesh::VAO.AddAttrib( Mesh::VBO, Mesh::IBO, attrib );

        Shader::CreateShader();
    }

public:

    void render( JGL::Scene* sc, JGL::Framebuffer& fb )
    {
        Shader::Bind();
        Shader::SetUniform1i( "frameTexture", fb.BindColor() );
        Render( sc );
    }

public:

    constexpr const static std::array<float,16> mesh_data_VBO = {
        /* 2D pos */  /* UV   */
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,
    };

    constexpr const static std::array<uint32_t,6> mesh_data_IBO =
    {
        0, 1, 2,
        2, 3, 0,
    };

public:
    static constexpr const char* Vert = 
        "#version 330 core\n"
        "layout ( location = 0 ) in vec2 pos;\n"
        "layout ( location = 1 ) in vec2 uv;\n"
        "\n"
        "out vec2 UV;\n"
        "void main() {\n"
        "   gl_Position = vec4( pos.x, pos.y, 0.0f, 1.0f );\n"
        "   UV = uv;\n"
        "}";

    static constexpr const char* Frag = 
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 UV;\n"
        "uniform sampler2D frameTexture;\n"
        "void main() {\n"
        "   FragColor = texture( frameTexture, UV );\n"
        "   FragColor *= 0.2f;\n"
        "}";

public:
    JGL::Texture& mTexture;

};

class Simple 
    : public JGL::Scene
{
public:
    Simple() : Scene( FourthRiver::GetRenderContext() ), Frame(), mFrameObject( Frame )
    {  }

public:
    /* Objects */
    Cube cube;

    JGL::Framebuffer Frame;
    FrameObject mFrameObject;

public:

    void OnLoad() override
    {  
        auto size = GetContext().renderContext().Size();

        Frame.Bind();
        Frame.SetSize( size.width, size.height );
        Frame.ColorAttachment();
        Frame.DepthStencilAttachment();

        ASSERT( Frame.CheckBuffer(), "FUCK!" );

        Frame.Unbind();
    }

    void OnExit() override
    {  }


    void OnUpdate() override
    {
        if ( GetContext().GetKey( GLFW_KEY_TAB ) == GLFW_PRESS )
            Exit();

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        Frame.Bind();
        {
            glClearColor( 0.1, 0.1, 0.15, 1.0 );
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            glEnable( GL_DEPTH_TEST );

            /* Render Objects */
            cube.Render( this );
        }
        Frame.Unbind();

        mFrameObject.render( this, Frame );

        ImGui::ShowDemoWindow();

        /* ImGUI */
        ImGui::Begin("DEBUG WINDOW!");
        ImGui::Text("%.2fms (%i FPS)\n", GetContext().FrameTime, GetContext().FPS );

        ImGui::Text("VERTEX SHADER: \n\n%s\n\nFRAG SHADER\n\n%s", 
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

