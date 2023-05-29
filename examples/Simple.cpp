#include "FourthRiver.h"
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
    {
        using namespace FourthRiver::Components::PipelineModules;

        AddModule<Core_Vertex>();
        AddModule<Core_Fragment>();
        AddModule<MVP>();
        AddModule<Color>();
        AddModule<Texturer>( Texturer(
                    "testTexture",
                    "color *= texture ( testTexture, UV );",
                    textureSamplerUniforms
                ) );
        CreatePipeline();

        LoadSimpleFromFile( "assets/cube.obj", 0 );

        mTexture.SetImgDataF( "assets/textures/test.png", GL_TEXTURE_2D, GL_RGB );
        SetTexture( mTexture );

        ShaderPipeline::InitializeModules();
    }

    static void textureSamplerUniforms( FourthRiver::Object* obj )
    {
        auto& sh = obj->GetComponent<FourthRiver::Components::Shader>();

        sh.SetUniform1i("testTexture", 0);
    }

public:
    JGL::Texture mTexture;
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

