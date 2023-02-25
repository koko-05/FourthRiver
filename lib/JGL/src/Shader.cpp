#include "JGL/Shader.h"
#include "JGL/common.h"
#include <stdlib.h>
#include <malloc.h>
#include <fstream>


/* 
 * definitions for Shader abstraction layer
 * for OpenGL shaders
 * */

namespace JGL
{

Shader::Shader() : mUniformsMap()
{  }

Shader::~Shader()
{  
    if ( !gl_Program ) return;

    glDeleteProgram( gl_Program );
}

Shader::Shader( const Shader& v )
    : gl_Program( v.gl_Program ),
      mUniformsMap( v.mUniformsMap ),
      mVertexSource( v.mVertexSource ),
      mFragmentSource( v.mFragmentSource)
{  }

Shader Shader::operator=( const Shader& v )
{
    if ( &v == this ) return *this;

    Shader temp( v );
    std::swap(gl_Program, temp.gl_Program);
    std::swap(mUniformsMap, temp.mUniformsMap);
    std::swap(mVertexSource, temp.mVertexSource);
    std::swap(mFragmentSource, temp.mFragmentSource);
    return *this;
}


void Shader::CreateShaderS( const char* _vertexSource, const char* _fragSource)
{
    #if DEBUG_BUILD == 1
    {  
        mVertexSource   = std::move( std::string(_vertexSource) );
        mFragmentSource = std::move( std::string( _fragSource ) );
    }
    #endif

    ASSERT( _vertexSource, "Vertex Source cannot be nullptr!" );
    ASSERT( _fragSource, "Fragment Source cannot be nullptr!" );

    gl_Program = CreateShader( _vertexSource, _fragSource );
}

void Shader::CreateShaderF( const char* _filePath, const char _delim )
{
    ASSERT( _filePath, "Shader filepath cannot be null" );

    static char vertexSource[MAX_SHADER_SOURCE_SIZE + 1]   = { 0 };
    static char fragmentSource[MAX_SHADER_SOURCE_SIZE + 1] = { 0 };

    std::ifstream file( _filePath );
    if ( !file.is_open() ) ASSERT( false, "Error. Shader file could not be loaded." );

    file.get( vertexSource, MAX_SHADER_SOURCE_SIZE, _delim ); 
    if ( file.eof() )
        ASSERT( false, "Error occured while parsing shader! eof() enountered after getting vertex shader src" );
    
    file.get();
    file.get( fragmentSource, MAX_SHADER_SOURCE_SIZE, _delim );

    #if DEBUG_BUILD == 1
        mVertexSource   = vertexSource;
        mFragmentSource = fragmentSource;
    #endif

    gl_Program = CreateShader( vertexSource, fragmentSource );
}


void Shader::Bind() const
{
    glUseProgram( gl_Program ); 
}

void Shader::UnBind() const
{
    glUseProgram( 0 );
}

GLuint Shader::CompileShader( GLenum _type, const char* _src )
{
    GLuint id = glCreateShader( _type );
    glShaderSource( id, 1, &_src, NULL );
    glCompileShader( id );

    GLint result = 0;
    glGetShaderiv( id, GL_COMPILE_STATUS, &result );
    if ( result == GL_FALSE )
    {
        GLsizei length = 0;
        glGetShaderiv( id, GL_INFO_LOG_LENGTH, &length );

        char* message = (char*)alloca( sizeof (char) * length );
        glGetShaderInfoLog( id, length * sizeof(char), &length, message);

        std::cout << "Error to compile shader: " << std::endl;
        std::cout << message << std::endl;
    }

    return id;
}

GLuint Shader::CreateShader( const char* _vertexSrc, const char* _fragSrc )
{
    GLuint pId = glCreateProgram();
    GLuint vs = CompileShader( GL_VERTEX_SHADER, _vertexSrc );
    GLuint fs = CompileShader( GL_FRAGMENT_SHADER, _fragSrc );

    // link the two shaders together
    glAttachShader( pId, vs ); 
    glAttachShader( pId, fs );
    
    // TODO: read on these
    glLinkProgram( pId );
    glValidateProgram( pId );

    GLint res;
    glGetProgramiv( pId, GL_VALIDATE_STATUS, &res );
    if ( res == GL_FALSE )
        std::cout << "Something's wrong with the shader program. Validation failed";

    glDetachShader( pId, vs );
    glDetachShader( pId, fs );
    glDeleteShader( vs ); 
    glDeleteShader( fs ); 

    return pId;
}



GLint Shader::GetUniformLocation( const char* _uniformName )
{
    ASSERT( _uniformName, "Uniform name cannot be null!" );

    std::string s = _uniformName;
    auto i = mUniformsMap.find( s );
    GLint id = 0;

    if ( i != mUniformsMap.end() )
        id = (*i).second;
    else
    {
        id = glGetUniformLocation( gl_Program, _uniformName );
        if ( id == -1 )
        {
            DEBUG_PRINT( " Could not find uniform in shader ", "[WARN]", stdout );
            printf("%s\n", _uniformName);
        }
        else
            mUniformsMap[_uniformName] = id;
    }
    return id;
}


GLint Shader::SetUniform4f( const char* _uniformName, const JM::Vect4& _vals )
{
    GLint id = GetUniformLocation( _uniformName );
    glUniform4f( id, _vals.x(), _vals.y(), _vals.z(), _vals.w() );
    return id;
}

GLint Shader::SetUniform4f( GLint _uniformId, const JM::Vect4& _vals)
{
    glUniform4f( _uniformId, _vals.x(), _vals.y(), _vals.z(), _vals.w() );
    return _uniformId;
}

GLint Shader::SetUniform1i( const char* _uniformName, int _v )
{
    GLint id = GetUniformLocation( _uniformName );
    glUniform1i( id, _v );
    return id;
}

GLint Shader::SetUniform1i( GLint _uniformId,  int _v )
{
    glUniform1i( _uniformId, _v );
    return _uniformId;

}

GLint Shader::SetUniformMat4( const char* _uniformName, const JM::Mat<4,4>& _vals )
{
    GLint id = GetUniformLocation( _uniformName );
    glUniformMatrix4fv( id, 1, GL_FALSE, &_vals.mVals[0][0] );
    return id;
}

GLint Shader::SetUniformMat4( GLint _uniformId, const JM::Mat<4,4>& _vals)
{
    glUniformMatrix4fv( _uniformId, 1, GL_FALSE, &_vals.mVals[0][0] );
    return _uniformId;
}

}
