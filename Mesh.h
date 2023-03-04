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
  using ref = std::reference_wrapper;


public:
  template< typename vT, size_t vSize, typename iT, size_t iSize  >
  void LoadFromData( const std::array<vT,vSize>& verts, 
                     const std::array<iT,iSize>& indices, 
                     GLenum access = GL_STATIC_DRAW );

  void LoadFromFile( const char* filePath, size_t index, std::vector<FileVectors>& vects, std::vector<uint32_t>& indices );

  void CreateVertexBuffer( GLenum access, ... ); /* Args MUST follow pattern: std::vector, size_t elemSize*/
  void CreateIndexBuffer( std::vector<uint32_t>& indices );

public:
  struct FileData {
    std::optional<JM::Vect3> pos; 
    std::optional<JM::Vect2> uvs;
    std::optional<JM::Vect3> norms; 
  };

  static std::unordered_map<std::string, JM::Vect3> mFilesPosition;
  static std::unordered_map<std::string, JM::Vect3> mFilesNormals;
  static std::unordered_map<std::string, JM::Vect2> mFilesUVs;

private:

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

