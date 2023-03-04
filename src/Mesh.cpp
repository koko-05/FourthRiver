#include "Mesh.h"

namespace TigerEngine
{
namespace Components
{

void Mesh::Apply( JGL::Scene* sc )
{
    UNUSED( sc );
    JGL::Mesh* m = static_cast<JGL::Mesh*>(this);
    Object::mesh = m;
}


void Mesh::LoadFromFile( const char* filePath, size_t index, std::vector<FileVectors>& vects, std::vector<uint32_t>& indices)
{
    static char lineBuffer[256]= { 0 };

    if ( mFilesPosition[filePath].empty() )
        LoadVertexesFromFile( filePath, index );

    std::ifstream file( filePath );
    if ( !file.is_open() ) return;


    char first;
    std::unordered_map<uint32_t, uint32_t> indexMap;

    while ( !file.eof() )
    {
        memset( lineBuffer, 0, 256 );

        file.getline( lineBuffer, 256 );
        std::istringstream line( lineBuffer );

        line >> first;

        if ( first == 'o' )
        {
            std::string fname;
            line >> fname;
            strcpy( name, fname.c_str(), MAX_OSIZE );
            continue;
        }

        if ( first != 'f' ) continue;

        char vertexBuffer[16] = { 0 };
        line.getLine( vertexBuffer, 16, ' ' );

        for ( int i = 0; i < 16; i++ ) if ( vertexBuffer[i] == '/' ) vertexBuffer[i] = ' ';
        std::istringstream vStr( vertexBuffer );

        uint32_t indexVertex = 0, indexUv = 0, indexNormal = 0;
        vStr >> indexVertex;
        char w; vStr.get( w ); 

        if ( vStr.peek() == ' ' )
        {
            vStr << indexNormal;
            sendData( indexMap, indexVertex, indexUv, indexNormal, vects, indices );
            continue;
        }

        vStr << indexUv;

        if ( vStr.peek() == ' ' )
        {
            vStr << indexNormal;
            sendData( indexMap, indexVertex, indexUv, indexNormal, vects, indices );
            continue;
        }

        sendData( indexMap, indexVertex, indexUv, indexNormal, vects, indices );
    }
}

void Mesh::sendData( std::unordered_map<uint32_t,  uint32_t>& indexMap, uint32_t indexVertex, uint32_t indexUvs, uint32_t indexNormal, std::vector<FileData>& vects, std::vector<uint32_t>& indices )
{
    if ( !indexMap.contains(indexVertex) )
    {
        JM::Vect3 position = indexVertex ? mFilesPosition[filePath][indexVertex - 1] : std::nullopt;
        JM::Vect2 uv       = indexUv     ? mFilesUVs[filePath][indexUv - 1]          : std::nullopt;
        JM::Vect3 normal   = indexNormal ? mFilesNormal[filePath][indexNormal - 1]   : std::nullopt;
        vects.push_back( { position, uv, normal } );
        indexMap[ indexVertex ] = vects.size() - 1;
    }

    indices.push_back( indexMap[ indexVertex ] );
}

void Mesh::LoadVertexesFromFile( const char* filePath, size_t index )
{
    static char lineBuffer[256]= { 0 };

    std::ifstream file( filePath );
    if ( !file.is_open() )
    {
        DEBUG_PRINT( "Cannot open asset!", "WARN", stdout );
        return;
    }

    auto& verts = mFilesPosition[filePath];
    auto& uvs   = mFilesUVs[filePath];
    auto& norms = mFilesNormals[filePath];

    char first;
    while ( !file.eof() )
    {
        memset( lineBuffer, 0, 128 );

        file.getline( lineBuffer, 256 );
        std::istringstream line( lineBuffer );

        line >> first;

        if ( first != 'v' ) continue;

        char second = '\0';
        line >> second;

        if ( second == 't' )
        {
            JM::Vect2 uv;
            line >> uv.x() >> uv.y();
            uvs.push_back( uv );
        }

        if ( second == 'n' )
        {
            JM::Vect3 norm;
            line >> norm.x() >> norm.y() >> norm.z();
            norms.push_back( norm );
        }

        if ( second == ' ' )
        {
            JM::Vect3 pos;
            line >> pos.x() >> pos.y() >> pos.z();
            norms.push_back( pos );
        }

    }

}

void Mesh::CreateVertexBuffer( GLenum access, ... )
{
  constexpr auto IBOacc = GL_STATIC_DRAW;
  JGL::Mesh::VBO.Alloc( verts.size()   * sizeof(vT), GL_ARRAY_BUFFER, access, verts.data(),vSize );
  JGL::Mesh::IBO.Alloc( indices.size() * sizeof(iT), GL_ELEMENT_ARRAY_BUFFER, IBOacc, indices.data(),iSize);

  JGL::Mesh::Primitive = GL_TRIANGLES;

}

void CreateIndexBuffer( std::vector<uint32_t>& indices );


uint16_t Mesh::GetID() const
{
    return mId;
}

}
}
