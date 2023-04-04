#pragma once
#include "Component.h"

namespace JGL { class Scene; }

namespace FourthRiver
{

namespace Components
{

class Transform : public Component
{
public:
    using Matrix = JM::Matrix<4,4>;
    using Vect3  = JM::Vect3;

    static constexpr uint16_t mId = 1;

public: /* Convert all values into a single matrix */
    Matrix GetMatrix() const;

public: /* Static functions that convert individual values into matrices */
    static Matrix GetTranslation( const Vect3& amount );
    static Matrix GetRotation( const Vect3& rXYZ );
    static Matrix GetScale( const Vect3& sXYZ );

public:
    void Move( Vect3 amount );
    void Rotate( Vect3 degrees );

public:
    void Apply( JGL::Scene* scene ) override;
    void Merge( Object* dest, JGL::Scene* scene ) override;
    void Unmerge( Object* dest, JGL::Scene* scene ) override;
    uint16_t GetID() const override;
    void SetUniform( Object* o );

public:
   Vect3 Scale     = { 1.0f, 1.0f, 1.0f }; 
   Vect3 Rotation  = { 0.0f, 0.0f, 0.0f }; /* in radians */
   Vect3 Position  = { 0.0f, 0.0f, 0.0f }; 

private:
   Vect3 cScale; 
   Vect3 cRotation;
   Vect3 cPosition; 

};


}

}
