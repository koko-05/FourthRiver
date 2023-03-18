#include "../JMath.h"
#include <utility>
#include <memory.h>
#include <iostream>

namespace JM {

Projection::Projection( ProjectionType t, int w, int h )
{
    if ( t == ProjectionType::Orthographic )
    {
        Type = t;
        Orthographic.near    =  0.1f;
        Orthographic.far     =  100.0f;
        Orthographic.left    = -w;
        Orthographic.right   =  w;
        Orthographic.bottom  = -h;
        Orthographic.top     =  h;
    }
    else if ( t == ProjectionType::Perspective )
    {
        Type = t;
        Perspective.near    =  0.01f;
        Perspective.far     =  1000.0f;
        Perspective.width   =  w;
        Perspective.height  =  h;
        Perspective.fov     =  degToRad( 70.0f );
    }
}

Vect3 EulerAnglesToDirection( float pitch, float yaw )
{
    return
    {
        std::cos( degToRad(yaw) ) * std::cos( degToRad(pitch) ),
        std::sin( degToRad(pitch) ),
        std::sin( degToRad(yaw)) * std::cos( degToRad(pitch) )
    };
}

Mat<4,4> DoNothingMatrix()
{
    return {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    };
}


JM::Vect<3> Cross( const JM::Vect<3>& _a, const JM::Vect<3>& _b)
{
    JM::Vect<3> temp;
    temp.mVals[0] = ( ( _a.mVals[1] * _b.mVals[2] ) - ( _a.mVals[2] * _b.mVals[1] ) ); // x
    temp.mVals[1] = ( ( _a.mVals[2] * _b.mVals[0] ) - ( _a.mVals[0] * _b.mVals[2] ) ); // y
    temp.mVals[2] = ( ( _a.mVals[0] * _b.mVals[1] ) - ( _a.mVals[1] * _b.mVals[0] ) ); // z
    return temp;
}


}
