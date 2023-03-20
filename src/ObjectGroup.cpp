#include "ObjectGroup.h"
#include <fstream>


/*
 * TODO: BUG: a .obj's group resets index. We do not simulate that. In fact we ignore it. So you cant 
 * have a group with an unnamed object. I dissallow it. Not because Im lazy and dont want to rewrite
 * the entire Mesh class, but because we (I) intended for this to happen.
 *
 * */

namespace FourthRiver
{

void ObjectGroup::addObject( ObjectGroup::ref<Object>&& obj )
{
    mObjects.emplace_back( std::forward<ref<Object>&&>(obj) );
}

void ObjectGroup::addGroup( ObjectGroup::ref<ObjectGroup>&& grp, const char* gName )
{
    mGroups.emplace_back( std::forward<ref<ObjectGroup>&&>(grp) );
    auto& name = mGroups.back()->name;
    memcpy( name, gName, strlen( gName ) );
}

void ObjectGroup::Render( JGL::Scene* scene )
{
    for ( auto& g : mGroups )
        g->Render( scene );

    for ( auto& o : mObjects )
    {
        for ( size_t i = 0; i < ComponentManager::componentList.size; i++ )
            ComponentManager::componentList[i]->Merge( o.get(), scene );

        o->Render( scene );
    }
}

ObjectGroup::ObjectFileGroupData ObjectGroup::GetObjectIndicesFromFile( const char* fName, uint32_t index )
{
    static char lineBuffer[256]= { 0 };
    size_t currentObjectIndex = 0, currentGroupIndex = 0;
    ObjectFileGroupData ret;

    std::ifstream file( fName );
    if ( !file.is_open() ) return ret;

    char first;
    bool definesUnnamedObject = false;

    while ( !file.eof() )
    {
        memset( lineBuffer, 0, 256 );

        file.getline( lineBuffer, 256 );
        std::istringstream line( lineBuffer );

        line >> first; 

        if ( currentGroupIndex > index ) return ret;

        if ( first == 'f' && currentObjectIndex == 0 )
        {
            definesUnnamedObject = true;
            if ( currentGroupIndex == index )
            {
                ret.indices.push_back( currentObjectIndex );
                memcpy( ret.name, "UNNAMED", 8 );
            }
        }

        if ( first == 'o' )
        {
            if ( currentGroupIndex == index )
            {
                ret.indices.push_back( currentObjectIndex );

            }

            if ( !definesUnnamedObject )
                currentObjectIndex++;
            else
                definesUnnamedObject = false;
        }


        if (  first == 'g' && currentGroupIndex++ == index )
        {
            line.getline(ret.name, MAX_OSIZE);
        }

    }

    return ret;
}

}
