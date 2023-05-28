
namespace FourthRiver
{

LightContext::LightContext()
{
    CreateLightContext();
}

void LightContext::CreateLightContext()
{
    static const std::string colors 
        = "vec3 LightColors[" + std::to_string( MAX_LIGHTS ) + "];";

    AddElement( MAX_LIGHTS, 4 * 4, colors.c_str() );
    AddElement( 1, 4, "uint LightCount;" );

    CreateLightMacro();

    Create(0);
}

void LightContext::CreateLightMacro()
{
    ASSERT( o.glslCode, "GLSL code for lighting Uniform Element in UBO is null!" );

    MacroDef = "layout (std140) uniform FR_LightingBlock {\n"

    for ( auto& o : elements )
        MacroDef += std::string( o.glslCode ) + "\n";

    MacroDef += "};\n\n"
}

void LightContext::AddLight( void* data )
{
    auto info = (LightInformation*)data;

    Get<UBO::Array<JM::Vect3>>(0)[size]=info->color;
    Get<unsigned int>(elements.size() - 1) = ++size;

    UpdateBuffer();
}

}
