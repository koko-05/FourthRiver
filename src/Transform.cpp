#include "Transform.h"
#include "JGL/Scene.h"

/* 
 * Defines what is written in the header file 
 *
 * */

namespace TigerEngine
{

namespace Components
{

/* Transforms */
void Transform::Move( Vect3 amount )
{
    Position.x() += amount.x();
    Position.z() += amount.z();
    Position.y() += amount.y();
}

void Transform::Rotate( Vect3 degrees )
{
    Position.x() += JM::degToRad( degrees.x() );
    Position.z() += JM::degToRad( degrees.z() );
    Position.y() += JM::degToRad( degrees.y() );
}

Transform::Matrix Transform::GetMatrix() const
{
    return GetRotation( Rotation ) *
           GetTranslation( Position ) *
           GetScale( Scale );
}

uint16_t Transform::GetID() const
{
    return mId;
}

void Transform::Apply( JGL::Scene* scene )
{
    mvp_data = scene->GetCamera().Projection() *
               scene->GetCamera().View() *
               GetMatrix();
}


/* Matrix Creation */
Transform::Matrix Transform::GetTranslation( const Vect3& a )
{
    return {
        { 1.0f,    0.0f,    0.0f,    a.x() },
        { 0.0f,    1.0f,    0.0f,    a.y() },
        { 0.0f,    0.0f,    1.0f,    a.z() },
        { 0.0f,    0.0f,    0.0f,    1.0f }
    };
}

Transform::Matrix Transform::GetRotation( const Vect3& r )
{
    // TODO: optimize this

    using namespace std;

    Matrix rx = { 
        { 1.0f,   0.0f,        0.0f,       0.0f },
        { 0.0f,   cos(r.x()), -sin(r.x()), 0.0f },
        { 0.0f,   sin(r.x()),  cos(r.x()), 0.0f },
        { 0.0f,   0.0f,        0.0f,       1.0f }
    };

    Matrix ry = { 
        {  cos(r.y()), 0.0f,   sin(r.y()), 0.0f },
        {  0.0f,       1.0f,   0.0f,       0.0f },
        { -sin(r.y()), 0.0f,   cos(r.y()), 0.0f },
        { 0.0f,        0.0f,   0.0f,       1.0f }
    };

    Matrix rz = { 
        { cos(r.z()), -sin(r.z()), 0.0f, 0.0f },
        { sin(r.z()),  cos(r.z()), 0.0f, 0.0f },
        { 0.0f,        0.0f,       1.0f, 0.0f },
        { 0.0f,        0.0f,       0.0f, 1.0f }
    };

    return rx * ry * rz; 
}

Transform::Matrix Transform::GetScale( const Vect3& scale )
{
    return {
        { scale.x(), 0.0f,      0.0f,      0.0f },
        { 0.0f,      scale.y(), 0.0f,      0.0f },
        { 0.0f,      0.0f,      scale.z(), 0.0f },
        { 0.0f,      0.0f,      0.0f,      1.0f }
    };
}

}

}
