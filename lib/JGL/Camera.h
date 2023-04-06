#pragma once
#include "JM/JMath.h"

/* 
 * Class decleration for a camera, designed to be
 * used inside a scene
 * */
namespace JGL
{


class Camera
{
public:
    using Matrix = JM::Matrix<4,4>;
    using Vect3  = JM::Vect3;

    static constexpr float DEFAULT_FAR_PLANE  = 1000.0f;
    static constexpr float DEFAULT_NEAR_PLANE = 0.01f;
    static constexpr float DEFAULT_FOV        = 70.0f;

public:
    Camera( JM::ProjectionData projection );
    ~Camera() = default;

public:
    Matrix View() const;
    Matrix Projection() const;

    /* TODO: Getters */
    void SetPosition( Vect3 pos );
    void Move( Vect3 amount );
    void MoveWithDir( Vect3 amount );
    void SetDirection( float yaw, float pitch );
    void MoveDirection( float yaw, float pitch );

private:
    void calculateView();

public:
    static JM::ProjectionData DefaultCameraData( int w, int h );

public:
    Matrix mView;
    JM::ProjectionMatrix mProjection;

public: /* these shouldnt be modified directly. Be careful */
    float  mYaw = 0.0f, mPitch = 0.0f;
    Vect3  Position  = { 0 };
    Vect3  Direction = { 0 };
    Vect3  RightAxis = { 0 };
    Vect3  UpAxis    = { 0 };
};

}



