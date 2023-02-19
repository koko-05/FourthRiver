#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>


#ifdef DEBUG_BUILD
  #undef DEBUG_BUILD
  #define DEBUG_BUILD 1
#else
  #define DEBUG_BUILD 0
#endif

#define DEBUG_PRINT( message, suffix, file ) do { if ( DEBUG_BUILD ) { \
    fprintf( file, "%s[%s][%s][%d] -> %s\n", suffix, __FILE__, __FUNCTION__, __LINE__, message ); } } while ( 0 )


#define ASSERT( condition, message ) do { if ( DEBUG_BUILD && !(condition) ) { \
    DEBUG_PRINT( message, "\n\n[ERROR][ASSERTION FAILED]", stderr); std::cin.get(); abort(); } } while ( 0 )

#define UNUSED( var ) (void)(var); 
#define UNIMPLEMENTED ASSERT( true, "Unimplemented function called!" )

#define TODO( comment ) DEBUG_PRINT( comment, "[INFO][TODO]", stdout ) 

// countof. Requieres valid array
#define countof( array ) ( sizeof ( array ) / sizeof( array[0] ) )



GLuint GetGLSize( GLenum _type );

/*
This used to be a function. But specializing global function templates is a pain in the ass for the linker
I could not find a way for this to link; it would always give me duplicate definitiuons from all the #includes
I'm told that qoute "Because template definitions often appear in header files, a template definition might 
be repeated in several compilation units. All definitions, however, must be the same. This restriction is
called the One-Definition Rule". So I guess it's not possible to do this. Sad, I really liked how it looked in code

template< class _T >
GLenum GetValidGLTypeEnum() { UNIMPLEMENTED; }

template<> GLenum GetValidGLTypeEnum<float>()          { return GL_FLOAT; }
template<> GLenum GetValidGLTypeEnum<double>()         { return GL_DOUBLE; }
template<> GLenum GetValidGLTypeEnum<char>()           { return GL_BYTE; }
template<> GLenum GetValidGLTypeEnum<int>()            { return GL_INT; }
template<> GLenum GetValidGLTypeEnum<short>()          { return GL_SHORT; }
template<> GLenum GetValidGLTypeEnum<unsigned char>()  { return GL_UNSIGNED_BYTE; }
template<> GLenum GetValidGLTypeEnum<unsigned int>()   { return GL_UNSIGNED_INT; }
template<> GLenum GetValidGLTypeEnum<unsigned short>() { return GL_UNSIGNED_SHORT; }
*/
 
