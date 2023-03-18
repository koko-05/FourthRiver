#include "Mesh.h"
#include <sstream>
#include <fstream>

namespace TigerEngine
{
namespace Components
{

namespace Files
{
  std::unordered_map<std::string, std::vector<JM::Vect3>> FilePositions;
  std::unordered_map<std::string, std::vector<JM::Vect3>> FileNormals;
  std::unordered_map<std::string, std::vector<JM::Vect2>> FileUVs;
}

Mesh::Mesh()
{
    JGL::Mesh::VBO.setTarget( GL_ARRAY_BUFFER );
    JGL::Mesh::IBO.setTarget( GL_ELEMENT_ARRAY_BUFFER );
}

void Mesh::Apply( JGL::Scene* sc )
{
    UNUSED( sc );
    JGL::Mesh* m = static_cast<JGL::Mesh*>(this);
    Object::mesh = m;
}

void Mesh::CreateAttributeFromFileData( std::vector<FileData>& vect, JGL::VertexAttribute& attrib )
{
    auto& o = vect[0];

    if ( o.pos )   attrib.Add( GL_FLOAT, 3 );
    if ( o.uvs )   attrib.Add( GL_FLOAT, 2 );
    if ( o.norms ) attrib.Add( GL_FLOAT, 3 );
}

void Mesh::LoadFromFile( const char* filePath, size_t index, std::vector<FileData>& vects, std::vector<uint32_t>& indices)
{
    static char lineBuffer[256]= { 0 };

    if ( Files::FilePositions[filePath].empty() )
        LoadVertexesFromFile( filePath );

    std::ifstream file( filePath );
    if ( !file.is_open() ) return;

    char first;
    bool definesUnnamedObject = false;
    uint32_t hitCount = 0, oCount = 0;
    std::unordered_map<uint32_t, uint32_t> indexMap;
    while ( !file.eof() )
    {
        memset( lineBuffer, 0, 256 );

        file.getline( lineBuffer, 256 );
        std::istringstream line( lineBuffer );

        line >> first;

        if ( first == 'f' && oCount == 0 )
            definesUnnamedObject = true;

        if ( first == 'o' )
        {
            if ( oCount > 0 || definesUnnamedObject || hitCount )
            {
                oCount++;
                if ( oCount > index ) return;
            }
            else
                hitCount++;

        }

        if ( first == 'o' && oCount == index )
        {
            std::string fname;
            line >> fname;
            strncpy( name, fname.c_str(), MAX_OSIZE );
            continue;
        }

        if ( oCount != index ) continue;
        if ( first != 'f' )    continue;

        char junk;
        line.get( junk );
        for ( int c = 0; c < 3; c++ )
        {
            char vertexBuffer[16] = { 0 };
            line.getline( vertexBuffer, 16, ' ' );

            for ( int i = 0; i < 16; i++ ) if ( vertexBuffer[i] == '/' ) vertexBuffer[i] = ' ';
            std::istringstream vStr( vertexBuffer );

            uint32_t indexVertex = 0, indexUv = 0, indexNormal = 0;
            vStr >> indexVertex;

            if ( vStr.eof() || vStr.peek() != ' ' ) 
            {
                sendData( indexMap, indexVertex, 0, 0, vects, indices, filePath );
                continue;
            }
            else
            {
                vStr.get( junk ); 
                if ( !vStr.eof() && vStr.peek() == ' ' )
                {
                    vStr >> indexNormal;
                    sendData( indexMap, indexVertex, 0, indexNormal, vects, indices, filePath );
                    continue;
                }

                vStr >> indexUv;

                if ( !vStr.eof() && vStr.peek() == ' ' )
                    vStr >> indexNormal;

                sendData( indexMap, indexVertex, indexUv, indexNormal, vects, indices, filePath );
                continue;
            }
        }
    }
}

void Mesh::sendData( std::unordered_map<uint32_t,  uint32_t>& indexMap, uint32_t indexVertex, uint32_t indexUvs, uint32_t indexNormal, std::vector<FileData>& vects, std::vector<uint32_t>& indices, const char* filePath )
{
    if ( !indexVertex ) return;

    if ( indexVertex - 1 >= Files::FilePositions[filePath].size() )
        std::cout << " FUCK, a PROBLEM has OCCURED!!!! COUNT: " <<  Files::FilePositions[filePath].size() << "INDEX: " << indexVertex - 1 << std::endl;

    if ( !indexMap.contains(indexVertex) )
    {
        auto position = indexVertex ? 
            std::make_optional(Files::FilePositions[filePath][indexVertex - 1]) : std::nullopt;
        auto uv       = indexUvs    ?
            std::make_optional(Files::FileUVs[filePath][indexUvs - 1]) : std::nullopt;
        auto normal   = indexNormal ?
            std::make_optional(Files::FileNormals[filePath][indexNormal - 1]) : std::nullopt;

        vects.push_back( FileData( position, uv, normal ) );
        indexMap[ indexVertex ] = vects.size() - 1;
    }

    indices.push_back( indexMap[ indexVertex ] );
}

void Mesh::LoadVertexesFromFile( const char* filePath )
{
    static char lineBuffer[256]= { 0 };

    std::ifstream file( filePath );
    if ( !file.is_open() )
    {
        DEBUG_PRINT( "Cannot open asset!", "WARN", stdout );
        return;
    }

    auto& verts = Files::FilePositions[filePath];
    auto& uvs   = Files::FileUVs[filePath];
    auto& norms = Files::FileNormals[filePath];

    char first;
    while ( !file.eof() )
    {
        memset( lineBuffer, 0, 256 );

        file.getline( lineBuffer, 256 );
        std::istringstream line( lineBuffer );

        line >> first;

        if ( first != 'v' ) continue;

        char second = '\0';
        line.get( second );

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
            verts.push_back( pos );
        }

    }
}

Mesh::FileData::uniq_ptr Mesh::FileData::GetDataFromVector( std::vector<Mesh::FileData>& vect, size_t& out_size, size_t& out_eSize )
{
    size_t elementSize = 0;
    auto& o = vect[0];
    elementSize += o.pos   ? sizeof( o.pos.value() ) : 0;
    elementSize += o.uvs   ? sizeof( o.uvs.value() ) : 0;
    elementSize += o.norms ? sizeof( o.norms.value() ) : 0;

    uniq_ptr ptr( malloc( elementSize * vect.size() ), free );
    auto c_ptr = (char*)ptr.get();

    for ( size_t i = 0; i < vect.size(); i++ )
    {
        if ( vect[i].pos ) 
            ((JM::Vect3*)((size_t)c_ptr + i * elementSize + 0 ))[0] = vect[i].pos.value();
        if ( vect[i].uvs ) 
            ((JM::Vect2*)((size_t)c_ptr + i * elementSize + sizeof( JM::Vect3 ) ))[0] = vect[i].uvs.value();
        if ( vect[i].norms ) 
            ((JM::Vect3*)((size_t)c_ptr + i * elementSize + sizeof(JM::Vect3) + sizeof(JM::Vect2)))[0] = vect[i].norms.value();
    }

    out_size  = elementSize * vect.size();
    out_eSize = elementSize;

    return ptr;
}


void Mesh::CreateVertexBuffer( GLenum access, int num, ... )
{
    va_list args;

    va_start( args, num );
    void*  finalData = nullptr;
    size_t finalSize = 0;
    size_t finalElementSize = 0;

    for ( int i = 0; i < num / 3; i++ )
    {
        auto data   = va_arg( args, void* );
        auto size   = va_arg( args, size_t );
        auto e_size = va_arg( args, size_t );

        finalData = realloc( finalData, finalSize + size );
        memcpy( (char*)((size_t)finalData + finalSize), data, size );

        finalElementSize += e_size;
        finalSize        += size;
    }

    VAO.Bind();
    VBO.Alloc( finalSize, GL_ARRAY_BUFFER, access, finalData, finalSize / finalElementSize );
    Primitive = GL_TRIANGLES;
}

void Mesh::CreateIndexBuffer( GLenum access, void* indices, size_t size, size_t elemSize )
{
    VAO.Bind();
    IBO.Alloc( size, GL_ELEMENT_ARRAY_BUFFER, access, indices, size / elemSize);
}


uint16_t Mesh::GetID() const
{
    return mId;
}

}
}
