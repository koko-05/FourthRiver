#pragma once
#include "JM/JMath.h"
#include "JGL/Transform.h"


/* 
 * Class decleration for a camera, designed to be
 * used inside a scene
 * */
namespace JGL
{

class Camera
{
public:
    using Matrix = JM::Mat<4,4>;
    using Vect3  = JM::Vect3;

public:
    Camera( JM::ProjectionType proj, int w, int h );
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
    Matrix mProjection;
    Matrix mView;
    JM::ProjectionType mProjType;

public: /* these shouldnt be modified directly. Be careful */
    float  mYaw = 0.0f, mPitch = 0.0f;
    Vect3  Position;
    Vect3  Direction;
    Vect3  RightAxis;
    Vect3  UpAxis;
};

}



