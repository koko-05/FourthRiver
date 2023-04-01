#include "JMath.h"

namespace JM
{

constexpr Vector<3> euler2dir( float pitch, float yaw )
{
    return
    {
        std::cos( deg2rad(yaw) ) * std::cos( deg2rad(pitch) ),
        std::sin( deg2rad(pitch) ),
        std::sin( deg2rad(yaw)) * std::cos( deg2rad(pitch) )
    };
}

constexpr Matrix<4,4> DoNothingMatrix()
{
  return {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

constexpr float deg2rad( float degs )
{
    return degs * ( JM_PI / 180.0f );
}

constexpr ProjectionMatrix Projection_Perspective( ProjectionData d )
{
  float rl = d.r - d.l, tb = d.t - d.b, fn = d.f - d.n;

  return { 
    (2.0f * d.n) / rl, 0.0f,             ( d.r + d.l ) / rl,    0.0f,
    0.0f,             (2.0f * d.n) / tb, ( d.t + d.b ) / tb,    0.0f,
    0.0f,              0.0f,            -( d.f + d.n ) / fn, -( d.f * d.n ) / fn,
    0.0f,              0.0f,              -1.0f,                0.0f
  };
}

constexpr ProjectionMatrix Projection_Orthographic( ProjectionData d )
{
  float rl = d.r - d.l, tb = d.t - d.b, fn = d.f - d.n;

  return {
    2.0f / rl, 0.0f,      0.0f,      -( d.r + r.l ) / rl, 
    0.0f,      2.0f / tb, 0.0f,      -( d.t + d.b ) / tb,
    0.0f,      0.0f,     -2.0f / fn, -( d.f + d.n ) / fn,
    0.0f,      0.0f,      0.0f,       1.0f
  };
}

constexpr Cross( const Vect3& a, const Vect3& b )
{
    return {
      ( ( a.y * _b.z ) - ( _a.z * _b.y ) ),
      ( ( a.z * _b.x ) - ( _a.x * _b.z ) ),
      ( ( a.x * _b.y ) - ( _a.y * _b.x ) )
    };
}

}

}
