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

/* Vector 2 */

constexpr Vect2::Vect2( real _x )
  : x( _x ), y( _x )
{  }

constexpr Vect2::Vect2( real _x, real _y )
  : x( _x ), y( _y )
{}

constexpr Vect2::Vect2( Vector<2> v )
  : x( v[0] ), y( v[1] )
{}

Vect2 Vect2::Add( const Vect2& v ) const
{
  return { x + v.x, y + v.y };
}

Vect2 Vect2::operator+( const Vect2& v ) const
{
  return Add( v );
}

Vect2 Vect2::Sub( const Vect2& v ) const
{
  return { x - v.x, y - v.y };
}

Vect2 Vect2::operator-( const Vect2& v ) const
{
  return Sub( v );
}

Vect2 Vect2::Multiply( const Vect2& v ) const
{
  return { x * v.x, y * v.y };
}

Vect2 Vect2::operator*( const Vect2& v ) const
{
  return Multiply( v );
}

Vect2 Vect2::Divide( const Vect2& v ) const
{
  return { x / v.x, y / v.y };
}

Vect2 Vect2::operator/( const Vect2& v ) const
{
  return Divide( v );
}

Vect2& Vect2::PlusEquals( const Vect2& v )
{
  x += v.x; y += v.y;
  return *this;
}

Vect2& Vect2::operator+=( const Vect2& v )
{
  return PlusEquals( v );
}

Vect2& Vect2::MinusEquals( const Vect2& v )
{
  x -= v.x; y -= v.y;
  return *this;
}

Vect2& Vect2::operator-=( const Vect2& v )
{
  return MinusEquals( v );
}

Vect2& Vect2::MultEquals( const Vect2& v )
{
  x *= v.x; y *= v.y;
  return *this;
}

Vect2& Vect2::operator*=( const Vect2& v )
{
  return MultEquals( v );
}

Vect2& Vect2::DivideEquals( const Vect2& v )
{
  x *= v.x; y /= v.y;
  return *this;
}

Vect2& Vect2::operator/=( const Vect2& v )
{
  return DivideEquals( v );
}

/* Vector 3 */
constexpr Vect3::Vect3( real _x )
  : x( _x ), y( _x ), z( _x )
{  }

constexpr Vect3::Vect3( real _x, real _y )
  : x( _x ), y( _y ), z( _x )
{  }
constexpr Vect3::Vect3( real _x, real _y, real _z )
  : x( _x ), y( _y ), z( _z )
{  }

constexpr Vect3::Vect3( Vector<3> v )
  : x( v[0] ), y( v[1] ), z( v[2] )
{  }

Vect3 Vect3::Add( const Vect3& v ) const
{
  return { x + v.x, y + v.y, z + v.z };
}

Vect3 Vect3::operator+( const Vect3& v ) const
{
  return Add( v );
}

Vect3 Vect3::Sub( const Vect3& v ) const
{
  return { x - v.x, y - v.y, z - v.z };
}

Vect3 Vect3::operator-( const Vect3& v ) const
{
  return Sub( v );
}

Vect3 Vect3::Multiply( const Vect3& v ) const
{
  return { x * v.x, y * v.y, z * v.z };
}

Vect3 Vect3::operator*( const Vect3& v ) const
{
  return Multiply( v );
}

Vect3 Vect3::Divide( const Vect3& v ) const
{
  return { x / v.x, y / v.y, z / v.z };
}

Vect3 Vect3::operator/( const Vect3& v ) const
{
  return Divide( v );
}

Vect3& Vect3::PlusEquals( const Vect3& v )
{
  x += v.x; y += v.y; z += v.z;
  return *this;
}

Vect3& Vect3::operator+=( const Vect3& v )
{
  return PlusEquals( v );
}

Vect3& Vect3::MinusEquals( const Vect3& v )
{
  x -= v.x; y -= v.y; z -= v.z;
  return *this;
}

Vect3& Vect3::operator-=( const Vect3& v )
{
  return MinusEquals( v );
}

Vect3& Vect3::MultEquals( const Vect3& v )
{
  x *= v.x; y *= v.y; z *= v.z;
  return *this;
}

Vect3& Vect3::operator*=( const Vect3& v )
{
  return MultEquals( v );
}

Vect3& Vect3::DivideEquals( const Vect3& v )
{
  x *= v.x; y /= v.y; z /= v.z;
  return *this;
}

Vect3& Vect3::operator/=( const Vect3& v )
{
  return DivideEquals( v );
}

/* Vect 4 */
constexpr Vect4::Vect4( real _x )
  : x( _x ), y( _x ), z( _x ), w( _x )
{  }

constexpr Vect4::Vect4( real _x, real _y )
  : x( _x ), y( _y ), z( _x ), w( _y )
{  }

constexpr Vect4::Vect4( real _x, real _y, real _z )
  : x( _x ), y( _y ), z( _z ), w( _x )
{  }

constexpr Vect4::Vect4( real _x, real _y, real _z, real _w )
  : x( _x ), y( _y ), z( _x ), w( _w )
{  }

constexpr Vect4::Vect4( Vector<4> v )
  : x( v[0] ), y( v[1] ), z( v[2] ), w( v[3] )
{  }

Vect4 Vect4::Add( const Vect4& v ) const
{
  return { x + v.x, y + v.y, z + v.z, w + v.w };
}

Vect4 Vect4::operator+( const Vect4& v ) const
{
  return Add( v );
}

Vect4 Vect4::Sub( const Vect4& v ) const
{
  return { x - v.x, y - v.y, z - v.z, w - v.w };
}

Vect4 Vect4::operator-( const Vect4& v ) const
{
  return Sub( v );
}

Vect4 Vect4::Multiply( const Vect4& v ) const
{
  return { x * v.x, y * v.y, z * v.z, w * v.w };
}

Vect4 Vect4::operator*( const Vect4& v ) const
{
  return Multiply( v );
}

Vect4 Vect4::Divide( const Vect4& v ) const
{
  return { x / v.x, y / v.y, z / v.z, w / v.w };
}

Vect4 Vect4::operator/( const Vect4& v ) const
{
  return Divide( v );
}

Vect4& Vect4::PlusEquals( const Vect4& v )
{
  x += v.x; y += v.y; z += v.z; w += v.w;
  return *this;
}

Vect4& Vect4::operator+=( const Vect4& v )
{
  return PlusEquals( v );
}

Vect4& Vect4::MinusEquals( const Vect4& v )
{
  x -= v.x; y -= v.y; z -= v.z; w -= v.w;
  return *this;
}

Vect4& Vect4::operator-=( const Vect4& v )
{
  return MinusEquals( v );
}

Vect4& Vect4::MultEquals( const Vect4& v )
{
  x *= v.x; y *= v.y; z *= v.z; w *= v.w;
  return *this;
}

Vect4& Vect4::operator*=( const Vect4& v )
{
  return MultEquals( v );
}

Vect4& Vect4::DivideEquals( const Vect4& v )
{
  x *= v.x; y /= v.y; z /= v.z; w /= v.w;
  return *this;
}

Vect4& Vect4::operator/=( const Vect4& v )
{
  return DivideEquals( v );
}

}
