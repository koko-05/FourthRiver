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
    class ref : public std::unique_ptr<T> {};

public:
    ObjectGroup() {}

public:
    void addObject( ref<Object>&& obj );
    void addGroup( ObjectGroup::ref<ObjectGroup>&& grp, const char* gName );

public:
    struct ObjectFileGroupData { 
        std::vector<uint32_t> indices;
        char                  name[MAX_OSIZE] = {0};
    };

    struct GroupFileData
    {
        struct data
        { std::string filePath; uint32_t index; };

        std::vector<data> objects;
        std::vector<GroupFileData> groups;
        char name[MAX_OSIZE] = {0};
    };

    ObjectFileGroupData GetObjectIndicesFromFile( const char* fName, uint32_t gIndex );
    GroupFileData ParseGroupFile( const char* fName );
    template<typename ObjectClass, typename GroupClass, typename... _T>
    void LoadFromParsedGroupFile( GroupFileData& d, ObjectGroup& o, _T&&... args );

public:
    /* ObjectClass NEEDS to inheret from Mesh component. */
    template<typename ObjectClass, typename GroupClass, typename... _T>
    void LoadFromGroupFile( const char* fName, _T&&... args );

    /* ObjectClass NEEDS to inheret from Mesh component. */
    template<typename ObjectClass, typename... _T>
    void LoadGroupFromObjectFile( const char* fName, uint32_t index, _T&&... args ); 
    

public:

    template<typename T>
    T& GetObjectAs( size_t index )
    { return *dynamic_cast<T*>(mObjects[index].get()); }

    template<typename T>
    T& GetObjectByName( const char* name )
    {
        for ( size_t i = 0; i < mObjects.size(); i++ )
            if ( strncmp( name, mObjects[i]->name, MAX_OSIZE ) != 0 )
                return GetObjectAs<T>(i);
    }
    


public:
    void Render( JGL::Scene* scene ) override;

public:
    std::vector< ref<ObjectGroup> > mGroups;
    std::vector< ref<Object>      > mObjects;

private:
    int WhiteSpaceIgnore( std::istringstream& file );
};
}

/* template definitions */
namespace FourthRiver
{

template<typename ObjectClass, typename GroupClass, typename... _T>
void ObjectGroup::LoadFromGroupFile( const char* fName, _T&&... args )
{
    auto r = ParseGroupFile( fName );
    memcpy( r.name, fName, MAX_OSIZE );
    LoadFromParsedGroupFile<ObjectClass, GroupClass>( r, *this, std::forward<_T>(args)... );
}

template<typename ObjectClass, typename GroupClass, typename... _T>
void ObjectGroup::LoadFromParsedGroupFile( ObjectGroup::GroupFileData& d, ObjectGroup& o, _T&&... args )
{
    memcpy( o.name, d.name, MAX_OSIZE );
    for ( size_t i = 0; i < d.groups.size(); i++ )
    {
        o.mGroups.emplace_back( std::make_unique<GroupClass>() );
        LoadFromParsedGroupFile<ObjectClass, GroupClass>
            ( d.groups[i], *o.mGroups.back(), std::forward<_T>(args)...);
    }

    for ( auto& ob : d.objects )
    {
        auto obj = std::make_unique<ObjectClass>( std::forward<_T>(args)... );
        obj->Mesh::LoadSimpleFromFile( ob.filePath.c_str(), ob.index );
        auto ptr = static_cast<Object*>(obj.release());
        ref<Object> mobj; mobj.reset( ptr );
        o.addObject( std::move( mobj ) );
    }

}

template<typename ObjectClass, typename... _T>
void ObjectGroup::LoadGroupFromObjectFile( const char* fName, uint32_t index, _T&&... args )
{
    auto r = GetObjectIndicesFromFile( fName, index );
    memcpy( name, r.name, MAX_OSIZE );

    for ( uint32_t i : r.indices )
    {
        auto obj = std::make_unique<ObjectClass>( std::forward<_T>(args)... );
        obj->Mesh::LoadSimpleFromFile( fName, i );
        auto ptr = static_cast<Object*>(obj.release());
        ref<Object> mobj; mobj.reset( ptr );
        addObject( std::move( mobj ) );
    } // this is not how you use cpp pointers, i know
}

}
