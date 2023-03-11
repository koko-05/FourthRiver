#pragma once
#include <array>
#include <memory>
#include <optional>
#include <unordered_map>
#include "JM/JMath.h"
#include "JGL/Mesh.h"
#include "Component.h"


namespace TigerEngine
{
namespace Components
{

namespace Files
{
  extern std::unordered_map<std::string, std::vector<JM::Vect3>> FilePositions;
  extern std::unordered_map<std::string, std::vector<JM::Vect3>> FileNormals;
  extern std::unordered_map<std::string, std::vector<JM::Vect2>> FileUVs;
}

class Mesh
    : public Component,
      public JGL::Mesh
{
public:
  static constexpr uint16_t mId = 3;
  struct FileData;


public:
  template< typename vT, size_t vSize, typename iT, size_t iSize  >
  void LoadFromData( const std::array<vT,vSize>& verts, 
                     const std::array<iT,iSize>& indices, 
                     GLenum access = GL_STATIC_DRAW );

  void LoadFromFile( const char* filePath, size_t index, std::vector<FileData>& vects, std::vector<uint32_t>& indices );

  void CreateVertexBuffer( GLenum access, int num, ... ); /* Args MUST follow pattern: void* data, size_t size, size_t elem_size*/
  void CreateIndexBuffer( GLenum access, void* indices, size_t size, size_t elemSize );

private:

  void LoadVertexesFromFile( const char* filePath);
  void sendData( std::unordered_map<uint32_t,  uint32_t>& indexMap, uint32_t indexVertex, uint32_t indexUvs, uint32_t indexNormal, std::vector<FileData>& vects, std::vector<uint32_t>& indices, const char* filePath );

public:
  struct FileData {
    std::optional<JM::Vect3> pos; 
    std::optional<JM::Vect2> uvs;
    std::optional<JM::Vect3> norms; 

    using uniq_ptr = std::unique_ptr<void, void(*)(void*)>;

    static uniq_ptr GetDataFromVector( std::vector<FileData>& vect, size_t& out_size, size_t& out_eSize );
  };


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

