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

#define DEBUG_PRINT( message, suffix, file, ... ) do { if ( DEBUG_BUILD ) { \
    fprintf( file, "[%s][%s]: ", suffix, __FUNCTION__ );\
    fprintf( file, message, ##__VA_ARGS__ );} } while ( 0 )

#define ASSERT( condition, message, ... ) do { if ( DEBUG_BUILD && !(condition) ) { \
    DEBUG_PRINT( message, "\n\n[ERROR][ASSERTION FAILED]", stderr, ##__VA_ARGS__); abort(); } } while ( 0 )

#define UNUSED( var ) (void)(var); 
#define countof( array ) ( sizeof ( array ) / sizeof( array[0] ) )

GLuint GetGLSize( GLenum _type );

