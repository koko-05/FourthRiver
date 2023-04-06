#include "JMath.h"

namespace JM
{

Vector<3> euler2dir( float pitch, float yaw )
{
    return
    {
        std::cos( deg2rad(yaw) ) * std::cos( deg2rad(pitch) ),
        std::sin( deg2rad(pitch) ),
        std::sin( deg2rad(yaw)) * std::cos( deg2rad(pitch) )
    };
}

Matrix<4,4> DoNothingMatrix()
{
  return {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

float deg2rad( float degs )
{
    return degs * ( JM_PI / 180.0f );
}

ProjectionMatrix Projection_Perspective( ProjectionData d )
{
  ProjectionMatrix ret;
  ret.data   = d;
  float tans = std::tan( d.fov * 0.5f );

  ret.matrix = { 
    1.0f / ( tans * ( d.w / d.h ) ), 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f / tans, 0.0f, 0.0f,
    0.0f, 0.0f, ( d.n + d.f ) / ( d.n - d.f ), ( 2.0f * d.n * d.f ) / ( d.n - d.f ),
    0.0f, 0.0f, -1.0f, 0.0f
  };

  return ret;
}

ProjectionMatrix Projection( ProjectionData d )
{
  if ( d.isPerspective )
    return Projection_Perspective( d );
  else
    return Projection_Orthographic( d );

}

ProjectionMatrix Projection_Orthographic( ProjectionData d )
{
  ProjectionMatrix ret;
  ret.data = d;
  ret.data.isPerspective = false;

  float rl = d.r - d.l, tb = d.t - d.b, fn = d.f - d.n;

  ret.matrix = JM::Matrix<4,4>{
    2.0f / rl, 0.0f,      0.0f,      -( d.r + d.l ) / rl, 
    0.0f,      2.0f / tb, 0.0f,      -( d.t + d.b ) / tb,
    0.0f,      0.0f,     -2.0f / fn, -( d.f + d.n ) / fn,
    0.0f,      0.0f,      0.0f,       1.0f
  };

  return ret;
}

Vect3 Cross( const Vect3& a, const Vect3& b )
{
    return {
      ( ( a.y * b.z ) - ( a.z * b.y ) ),
      ( ( a.z * b.x ) - ( a.x * b.z ) ),
      ( ( a.x * b.y ) - ( a.y * b.x ) )
    };
}

/* Vector 2 */
Vect2 Vect2::Normalized() const
{
  return (*this) / Magnitude();
}

real Vect2::Magnitude() const
{
  return std::sqrt( x*x + y*y );
}

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
Vect3 Vect3::Normalized() const
{
  return (*this) / Magnitude();
}

real Vect3::Magnitude() const
{
  return std::sqrt( x*x + y*y + z*z );
}

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
Vect4 Vect4::Normalized() const
{
  return (*this) / Magnitude();
}

real Vect4::Magnitude() const
{
  return std::sqrt( x*x + y*y + z*z + w*w );
}

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
