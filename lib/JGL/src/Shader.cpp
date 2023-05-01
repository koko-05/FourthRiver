#include "JGL/Shader.h"
#include "JGL/common.h"
#include <stdlib.h>
#include <malloc.h>
#include <cstring>
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
      VertexSource( v.VertexSource ),
      FragmentSource( v.FragmentSource),
      mUniformsMap( v.mUniformsMap )
{  }

Shader Shader::operator=( const Shader& v )
{
    if ( &v == this ) return *this;

    Shader temp( v );
    std::swap(gl_Program, temp.gl_Program);
    std::swap(mUniformsMap, temp.mUniformsMap);
    std::swap(VertexSource, temp.VertexSource);
    std::swap(FragmentSource, temp.FragmentSource);
    return *this;
}


void Shader::CreateShaderS( const char* _vertexSource, const char* _fragSource)
{
    if ( gl_Program )
    {
        glDeleteProgram( gl_Program );
    }

    VertexSource   = std::move( std::string(_vertexSource) );
    FragmentSource = std::move( std::string( _fragSource ) );

    gl_Program = CreateShader( _vertexSource, _fragSource );
}

void Shader::CreateShaderF( const char* _filePath, const char _delim )
{
    if ( gl_Program )
    {
        glDeleteProgram( gl_Program );
    }

    static char vertexSource[MAX_SHADER_SOURCE_SIZE + 1]   = { 0 };
    static char fragmentSource[MAX_SHADER_SOURCE_SIZE + 1] = { 0 };

    std::ifstream file( _filePath );
    if ( !file.is_open() ) ASSERT( false, "Shader File cannot be loaded (%s)", _filePath );

    file.get( vertexSource, MAX_SHADER_SOURCE_SIZE, _delim ); 
    if ( file.eof() )
        ASSERT( false, "Error occured while parsing shader! eof() enountered after getting vertex shader src" );
    
    file.get();
    file.get( fragmentSource, MAX_SHADER_SOURCE_SIZE, _delim );

    VertexSource   = std::move(std::string(vertexSource));
    FragmentSource = std::move(std::string(fragmentSource));

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

        std::cout << "Error compiling shader: " << std::endl;
        std::cout << message << std::endl;
    }

    return id;
}

void Shader::SetMacroDef( char* m )
{
    MacroDef = m;
}

char* Shader::replaceMacros( const char* src )
{
    static const char macro[] = "FR_VAO_ATTRIBS";
    constexpr size_t sm = sizeof( macro ) / sizeof ( macro[0] );
    const size_t srs = strlen( src );
    const size_t mds = strlen( MacroDef );

    char* ptr = const_cast<char*>(src);
    while ( *++ptr )
    {
        auto p = ptr;
        size_t i = 0;
        for ( ; *p == macro[i]; i++ ) p++;

        if ( i + 1 == sm )
        {
            for ( auto pd = ptr; pd < sm + ptr; pd++ )
            {}

            auto newSrc = new char[ srs + mds - sm + 1 ];
            memcpy( newSrc, src, ptr - src );
            memcpy( newSrc + (ptr - src), MacroDef, mds );
            memcpy( newSrc + (ptr - src) + mds, ptr + sm, srs - ( ptr - src + sm ) );
            newSrc[srs + mds - sm] = '\0';

            return newSrc;
        }
    }

    return nullptr;
}

GLuint Shader::CreateShader( const char* _vertexSrc, const char* _fragSrc )
{
    auto _vs = replaceMacros( _vertexSrc );
    auto _fs = replaceMacros( _fragSrc   );

    auto vertexSource = _vs ? _vs : _vertexSrc;
    auto fragmentSource = _fs ? _fs : _fragSrc;

    GLuint pId = glCreateProgram();
    GLuint vs  = CompileShader( GL_VERTEX_SHADER, vertexSource );
    GLuint fs  = CompileShader( GL_FRAGMENT_SHADER, fragmentSource );

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

    if (_vs) delete[] _vs;
    if (_fs) delete[] _fs;

    return pId;
}



GLint Shader::GetUniformLocation( const char* _uniformName )
{
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
            DEBUG_PRINT( " Could not find uniform \"%s\"in shader ", "[WARN]", stdout, _uniformName);
        }
        else
            mUniformsMap[_uniformName] = id;
    }
    return id;
}


GLint Shader::SetUniform4f( const char* _uniformName, const JM::Vect4& _vals )
{
    GLint id = GetUniformLocation( _uniformName );
    glUniform4f( id, _vals.x, _vals.y, _vals.z, _vals.w );
    return id;
}

GLint Shader::SetUniform4f( GLint _uniformId, const JM::Vect4& _vals)
{
    glUniform4f( _uniformId, _vals.x, _vals.y, _vals.z, _vals.w );
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

GLint Shader::SetUniformMat4( const char* _uniformName, const JM::Matrix<4,4>& _vals )
{
    GLint id = GetUniformLocation( _uniformName );
    glUniformMatrix4fv( id, 1, GL_FALSE, &_vals.values[0][0] );
    return id;
}

GLint Shader::SetUniformMat4( GLint _uniformId, const JM::Matrix<4,4>& _vals)
{
    glUniformMatrix4fv( _uniformId, 1, GL_FALSE, &_vals.values[0][0] );
    return _uniformId;
}

}
