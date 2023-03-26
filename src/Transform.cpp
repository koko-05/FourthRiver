#include "Transform.h"
#include "JGL/Scene.h"
#include "ComponentManagerTemplates.cpp"

/* 
 * Defines what is written in the header file 
 *
 * */

namespace FourthRiver
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

    SetUniform( this );
}

void Transform::SetUniform( Object* o )
{
    if ( !o->shader ) return;

    o->shader->Bind();

    if ( o->flags & OBJECT_MVPID_CACHE_BIT )
    {
        o->shader->SetUniformMat4( 
            o->cached_MVPid, 
            o->mvp_data.Transpose()
        );
    }
    else
    {
        o->cached_MVPid = o->shader->GetUniformLocation( "uMVP" );
        o->flags |= OBJECT_MVPID_CACHE_BIT;
    }

}

void Transform::Merge( Object* dest, JGL::Scene* scene )
{
    auto c = dest->FindComponent<Transform>();
    if ( !c )
    {
        dest->mvp_data = 
            scene->GetCamera().Projection()  *
            scene->GetCamera().View()        *
            GetMatrix();

        SetUniform( dest );
        return;
    }

    auto cScale    = c->Scale;
    auto cPosition = c->Position;
    auto cRotation = c->Rotation;

    c->Scale    = c->Scale    * Scale;
    c->Position = c->Position + Position;
    c->Rotation = c->Rotation + Rotation;

    c->Apply( scene );

    c->Scale    = cScale;
    c->Position = cPosition;
    c->Rotation = cRotation;
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
