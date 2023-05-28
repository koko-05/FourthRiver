#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <string>
#include "JM/JMath.h"
#include "common.h"

#define MAX_SHADER_SOURCE_SIZE 1023

/* 
 * abstraction layer for OpenGL shader
 * creation, destruction and usage
 *
 * */

namespace JGL
{


class Shader
{
public: /* Initializing and destruction */
     Shader( );
     Shader( Shader&& v ) = default;
    ~Shader();

    Shader( const Shader& v );
    Shader operator=( const Shader& v );

public: /* shader creation OpenGL interface */
    void CreateShaderS( const char* _vertexSource, const char* _fragSource );
    void CreateShaderF( const char* _filePath, const char _delim );

    void Bind() const;
    void UnBind() const;

public:
    char* replaceMacros( const char* src, const std::string& macro, const std::string& MacroDef );

public: /* Uniform interface */
    GLint GetUniformLocation( const char* _uniformName );

    GLint SetUniform4f( const char* _uniformName, const JM::Vect4& _vals);
    GLint SetUniform4f( GLint _uniformId, const JM::Vect4& _vals );
    
    GLint SetUniform1i( const char* _uniformName, int _v );
    GLint SetUniform1i( GLint _uniformId,  int _v );

    GLint SetUniformMat4( const char* _uniformName, const JM::Matrix<4,4>& _vals );
    GLint SetUniformMat4( GLint _uniformId, const JM::Matrix<4,4>& _vals );

public: /* program index */
    GLuint gl_Program = 0;

private: /* compiling OpenGL private interface */
    GLuint CompileShader( GLenum _type, const char* _scr );
    GLuint CreateShader( const char* _vertexSrc, const char* _fragSrc );

protected:
    std::string mVertexSource;
    std::string mFragmentSource;

private: /* private member variables */
    std::unordered_map<std::string, GLint> mUniformsMap;
};


}
