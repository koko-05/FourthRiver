#include "JGL/Camera.h"

/* 
 * Definitions for camera class
 * */

namespace JGL
{

Camera::Camera( JM::ProjectionData proj )
    : mProjection( JM::Projection( proj ) )
{  
    SetDirection( 0.0f, 0.0f );
}

Camera::Matrix Camera::View() const
{
    return mView;
}

Camera::Matrix Camera::Projection() const
{
    return mProjection.matrix;
}

void Camera::calculateView()
{
    const Matrix translation = 
    {
        1.0f, 0.0f, 0.0f, -Position.x,
        0.0f, 1.0f, 0.0f, -Position.y,
        0.0f, 0.0f, 1.0f, -Position.z,
        0.0f, 0.0f, 0.0f, 1.0f         
    };

    Matrix transform = 
    {
         RightAxis.x, RightAxis.y, RightAxis.z, 0.0f,
         UpAxis.x,    UpAxis.y,    UpAxis.z,    0.0f,
         Direction.x, Direction.y, Direction.z, 0.0f,
         0.0f,          0.0f,          0.0f,          1.0f
    };

    mView = transform * translation;
}

void Camera::SetPosition( Camera::Vect3 pos )
{
    Position = pos;
    calculateView();
}

void Camera::Move( Camera::Vect3 amount )
{
    Position = Position + amount;
    calculateView();
}

void Camera::SetDirection( float yaw, float pitch )
{
    const float p = CLAMP(pitch, -89.0f, 89.0f);
    Direction = JM::euler2dir( p, yaw );
    Vect3 up  = { 0.0f, 1.0f, 0.0f };
    RightAxis = JM::Cross( up, Direction ).Normalized();
    UpAxis    = JM::Cross( Direction, RightAxis );

    mPitch = p;
    mYaw   = yaw;

    calculateView();
}

void Camera::MoveDirection( float yaw, float pitch )
{
    SetDirection( mYaw + yaw, mPitch + pitch );
}


}
