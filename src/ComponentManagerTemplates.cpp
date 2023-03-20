#include "ComponentManager.h"
#include "Component.h"

namespace FourthRiver
{
namespace Components
{


template<typename C>
C* ComponentManager::FindComponent()
{
    for ( auto it = componentList.begin(); it != componentList.end(); it++ )
    {
        if ( (*it)->GetID() == C::mId  )
            return static_cast<C*>(*it);
    }

    return nullptr;
}

template<typename C>
C& ComponentManager::GetComponent()
{ 
    return *(this); // implicit cast should take care of errors
}


}
}
