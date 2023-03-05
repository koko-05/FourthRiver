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


void Mesh::LoadFromFile( const char* filePath, size_t index, std::vector<FileData>& vects, std::vector<uint32_t>& indices)
{
    static char lineBuffer[256]= { 0 };

    if ( mFilesPosition[filePath].empty() )
        LoadVertexesFromFile( filePath, index );

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
                oCount++;
            else
                hitCount++;
        }

        if ( first == 'o' oCount == index )
        {
            std::string fname;
            line >> fname;
            strcpy( name, fname.c_str(), MAX_OSIZE );
            continue;
        }

        if ( oCount != index ) continue;
        if ( first != 'f' )    continue;

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

Mesh::FileData::uniq_ptr Mesh::FileData::GetDataFromVector( std::vector<Mesh::FileData>& vect, size_t& out_size, size_t& out_eSize )
{
    size_t elementSize;
    auto& o = vect[0];
    elementSize += o.pos   ? sizeof( o.pos.value() ) : 0;
    elementSize += o.uvs   ? sizeof( o.uvs.value() ) : 0;
    elementSize += o.norms ? sizeof( o.norms.value() ) : 0;

    uniq_ptr ptr( malloc( elementSize * vect.size() ), free );
    auto c_ptr = (char*)ptr.get();

    for ( size_t i = 0; i < vect.size(); i++ )
    {
        if ( vect[i].pos ) 
            ((JM::Vect3*)(ptr + i * elementSize + 0 ))[0] = vect[i].pos;
        if ( vect[i].uvs ) 
            ((JM::Vect2*)(ptr + i * elementSize + sizeof( JM::Vect3 ) ))[0] = vect[i].pos;
        if ( vect[i].uvs ) 
            ((JM::Vect2*)(ptr + i * elementSize + sizeof(JM::Vect3) + sizeof(JM::Vect2)))[0] = vect[i].pos;
    }

    out_size  = finalSize;
    out_eSize = finalElementSize;

    return std::move( ptr );
}


void Mesh::CreateVertexBuffer( GLenum access, int num, ... )
{
    va_list args;

    va_start( args, num );
    void*  finalData = nullptr;
    size_t finalSize = 0;
    size_t finalElementSize = 0;

    for ( size_t i = 0; i < num / 3; i++ )
    {
        auto data   = va_arg( args, void* );
        auto size   = va_arg( args, size_t );
        auto e_size = va_arg( args, size_t );

        realloc( finalData, finalSize + size );
        memcpy( (char*)finalData + (char*)finalSize, data, size );

        finalElementSize += e_size;
        finalSize        += size;
    }

    JGL::Mesh::VBO.Alloc( finalSize, GL_ARRAY_BUFFER, access, finalData, finalElementSize );
    JGL::Mesh::Primitive = GL_TRIANGLES;
}

void Mesh::CreateIndexBuffer( GLenum access, void* indices, size_t size, size_t elemSize )
{
    JGL::Mesh::IBO.Alloc( size, GL_ELEMENT_ARRAY_BUFFER, access, indices, elemSize);
}


uint16_t Mesh::GetID() const
{
    return mId;
}

}
}
