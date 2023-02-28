#pragma once
#include <array>
#include "JGL/Mesh.h"
#include "Component.h"


namespace TigerEngine
{
namespace Components
{

class Mesh
    : public Component,
      public JGL::Mesh
{
public:
  static constexpr uint16_t mId = 3;
  using VA = JGL::VertexAttribute;


public:
  template< typename vT, size_t vSize, typename iT, size_t iSize  >
  void LoadFromData( const std::array<vT,vSize>& verts, 
                     const std::array<iT,iSize>& indices, 
                     GLenum access = GL_STATIC_DRAW 
                   );

  VA LoadFromFile( const char* filePath, uint64_t index );
  VA LoadFromFile( const char* filePath, uint64_t index, 
                   uint64_t extraAlloc, void*(*populationFunc)(size_t objIndex, size_t vertIndex) );

public:
  void Apply( JGL::Scene* sc) override;
  uint16_t GetID() const override;
};

}
}

/* 
 * Template Definitions 
 * */

template< typename vT, size_t vSize, typename iT, size_t iSize  >
void TigerEngine::Components::Mesh::LoadFromData( const std::array<vT,vSize>& verts, const std::array<iT,iSize>& indices, GLenum access )
{
  constexpr auto IBOacc = GL_STATIC_DRAW;
  JGL::Mesh::VBO.Alloc( verts.size()   * sizeof(vT), GL_ARRAY_BUFFER, access, verts.data(),vSize );
  JGL::Mesh::IBO.Alloc( indices.size() * sizeof(iT), GL_ELEMENT_ARRAY_BUFFER, IBOacc, indices.data(),iSize);

  JGL::Mesh::Primitive = GL_TRIANGLES;
}

