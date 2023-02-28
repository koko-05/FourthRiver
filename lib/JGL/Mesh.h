#pragma once
#include <iostream>
#include "JGL/OpenGL.h"
#include "JGL/GPUBuffer.h"
#include "JGL/VertexArray.h"

// TODO: make this into its won file, get it from disk, etc 
// add dynamic geometry, add a combineMesh() that will allocate everything on 
// the CPU, and upload it onto the GPU once it has combined everyhting  ( for batch rendering  )etc 
namespace JGL
{

struct Mesh
{
    GPUBuffer   VBO;
    GPUBuffer   IBO;
    VertexArray VAO;

    GLenum      Primitive = GL_TRIANGLES;
};

}
