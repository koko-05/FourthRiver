#include "ObjectGroup.h"

namespace FourthRiver
{

void ObjectGroup::addObject( ref<Object>&& obj )
{
    mObject.push_back( std::forward(obj) );
}

void ObjectGroup::addGroup( const char* gName );
{
    mGroups.push_back();
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
            ComponentManager::componentList[i]->Merge( o.get() );

        o->Render( scene );
    }
}

ObjectGroup::groupsIndexData ObjectGroup::GetObjectIndicesFromFile( const char* fName, uint32_t index, size_t fileOffset)
{

}

}
