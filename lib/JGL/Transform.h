#pragma once
#include "JM/JMath.h"

/*
 * Decleration of Transform class
 * */
namespace JGL
{

class Transform
{
public:
    using Matrix = JM::Mat<4,4>;
    using Vect3  = JM::Vect3;

public:
    Transform() {}

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
   Vect3 Scale     = { 1.0f, 1.0f, 1.0f }; 
   Vect3 Rotation  = { 0.0f, 0.0f, 0.0f }; /* in radians */
   Vect3 Position  = { 0.0f, 0.0f, 0.0f }; 
};

}
