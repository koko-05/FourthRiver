#include "Component.h"

namespace FourthRiver
{
namespace Components
{

Component::Component()
{
    Object::componentList.push_back( this );
}

Component::~Component()
{
    Object::componentList.pop();
}

uint16_t Component::GetID() const
{
    return 0;
}

}

}
