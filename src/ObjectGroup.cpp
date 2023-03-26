#include "ObjectGroup.h"
#include <array>
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
    memcpy( name, gName, MAX_OSIZE );
}

void ObjectGroup::Render( JGL::Scene* scene )
{
    for ( auto& g : mGroups )
        g->Render( scene );

    for ( auto& o : mObjects )
    {
        for ( auto& c : o->componentList )
        {
            if ( !FindComponentByID( c->GetID() ) )
                c->Apply( scene );
        }

        for ( auto& c : ComponentManager::componentList )
        {
            c->Merge( o.get(), scene );
        }

        scene->Render( *o );
    }
}

int ObjectGroup::WhiteSpaceIgnore( std::istringstream& file )
{
    char c = ' ';
    while ( !file.eof() && isspace( c ) ) { file.get( c ); }
    return c;
}

ObjectGroup::GroupFileData ObjectGroup::ParseGroupFile( const char* fName )
{
    static char lineBuffer[256]= { 0 };
    std::array<int32_t, 32> gCount = { 0 };
    GroupFileData ret;

    std::ifstream file( fName );
    ASSERT( file.is_open(), "Cannot find .grp file: %s\n", fName );


    while ( !file.eof() )
    {
        char first = '0';
        size_t depth = 0;

        memset( lineBuffer, 0, 256 );
        file.getline( lineBuffer, 256 );
        std::istringstream line( lineBuffer );

        line.get(first); 

        while ( first == '\t' )
        {
            line.get(first);
            depth++;
        }

        if ( first == 'G' )
        {
            GroupFileData* ptr = &ret;
            for ( size_t i = 0; i < depth; i++ ) 
                ptr = &ptr->groups[gCount[i] - 1 == -1 ? 0 : gCount[i] - 1];

            ptr->groups.emplace_back();
            gCount[depth]++;
            line.getline( ptr->groups.back().name, MAX_OSIZE );
            continue;
        }

        if ( first == 'o' )
        {
            char fileName[128] = {0};
            ASSERT( line.get() == ' ', "Expected space after decleration of object (%s)", fName );
            ASSERT( WhiteSpaceIgnore( line ) == '\"', "Expected \" char after object (%s)", fName );

            line.getline( fileName, 128, '\"' );
            size_t index; line >> index;

            GroupFileData* ptr = &ret;
            for ( size_t i = 0; i < depth; i++ ) 
                ptr = &ptr->groups[gCount[i] - 1 == -1 ? 0 : gCount[i] - 1];

            ptr->objects.emplace_back( fileName, index );
            continue;
        }

        if ( first == 'g' )
        {
            char fileName[128] = {0};
            ASSERT( line.get() == ' ', "Expected space after decleration of obj group (%s)", fName );
            ASSERT( WhiteSpaceIgnore( line ) == '\"', "Expected \" char after obj grp (%s)", fName );

            line.getline( fileName, 128, '\"' );
            size_t index; line >> index;
            auto objects = GetObjectIndicesFromFile( fileName, index );

            GroupFileData* ptr = &ret;
            for ( size_t i = 0; i < depth; i++ ) 
                ptr = &ptr->groups[gCount[i] - 1 == -1 ? 0 : gCount[i] - 1];

            for ( auto& i : objects.indices )
                ptr->objects.emplace_back( fileName, i );
        }
        
    }

    return ret;
}

ObjectGroup::ObjectFileGroupData ObjectGroup::GetObjectIndicesFromFile( const char* fName, uint32_t index )
{
    static char lineBuffer[256]= { 0 };
    int64_t currentObjectIndex = 0, currentGroupIndex = -1;
    ObjectFileGroupData ret;

    std::ifstream file( fName );
    ASSERT( file.is_open(), "Cannot find .obj group file: %s\n", fName );

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


        if (  first == 'g' && ++currentGroupIndex == index )
        {
            memset( ret.name, 0, MAX_OSIZE );
            line.getline(ret.name, MAX_OSIZE);
        }

    }

    return ret;
}

}
