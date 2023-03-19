#pragma once
#include <memory>
#include <algorithm>
#include <vector>
#include "Mesh.h"
#include "Object.h"
#include "JM/JMath.h"

/*
 * Defines a ObjectGroup that will hold different objects
 * and Combine (apply) a component to each one
 *
 * Must be Loaded from file
 *
 * */

namespace JGL { class Scene; }

namespace FourthRiver
{

class ObjectGroup
    : public virtual Object
{
public:
    template<typename T>
    class ref : std::unique_ptr<T> {};

public:
    ObjectGroup() {}

public:
    void addObject( ref<Object>&& obj );
    void addGroup( const char* gName );

public:
    struct groupsIndexData { 
        std::vector<uint32_t> indices;
        uint32_t              gCount;
        size_t                fileOffset;
        char[MAX_OSIZE]       name;
    };

    groupsIndexData GetObjectIndicesFromFile( const char* fName, uint32_t index, size_t fileOffset);
    
    template<typename ObjectClass, typename _T>
    void LoadGroupFromFile( const char* fName, uint32_t index, size_t offset, T&&... args ); 
    /* ObjectClass NEEDS to inheret from Mesh component. 
     * Offset shall be 0, used internally */

public:
    void Render( JGL::Scene* scene ) override;

public:
    std::vector< ref<ObjectGroup> > mGroups;
    std::vector< ref<Object>      > mObjects;
};
}

/* template definitions */
namespace FourthRiver
{

template<typename ObjectClass, typename... _T>
void ObjectGroup::LoadGroupFromFile<ObjectClass>( const char* fName, uint32_t index, size_t offset, _T&&... args )
{
    // TODO: implement mergeAll
    auto r = GetObjectIndicesFromFile( fName, index, 0 );
    memcpy( name, r.name, strlen( r.name ) );

    for ( size_t i = 0; i < r.gCount; i++ )
    {
        mGroups.push_back();
        mGroups.back()->
            LoadGroupFromFile<ObjectClass>( fName, i, mergeAll, r.fileOffset, std::forward<_T>(args)... );
    }

    mObjects.reserve( r.indices.size() );
    for ( uint32_t i : r.indices )
    {
        auto obj = std::make_unique<ObjectClass>( std::forward<_T>(args)... );
        obj->Mesh::LoadSimpleFromFile( fPath, i );
        addObject( std::move( static_cast<ref<Object>>(obj) ) );
    }
}

}
