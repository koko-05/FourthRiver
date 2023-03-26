#include "ComponentManager.h"
#include "Component.h"

namespace FourthRiver
{
namespace Components
{


template<typename C>
C* ComponentManager::FindComponent()
{
    for ( size_t i = 0; i < componentList.size; i++ )
    {
        if ( componentList[i]->GetID() == C::mId  )
            return static_cast<C*>(componentList[i]);
    }

    return nullptr;
}

template<typename C>
C& ComponentManager::GetComponent()
{ 
    for ( size_t i = 0; i < componentList.size; i++ )
    {
        if ( componentList[i]->GetID() == C::mId  )
            return *static_cast<C*>(componentList[i]);
    }

    ASSERT( 0, "Could not find component!" );
}


}
}
