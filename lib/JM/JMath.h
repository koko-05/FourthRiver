#pragma once
#include <iostream>

#ifdef _DEBUG
#define JMDEBUG
#endif

#define JM_PI 3.14159265359
#define CLAMP( val, min, max ) ( val > max ? max : ( val < min ? min : val ) )
#define OPERATION( prefix, op, name, ... ) \
  prefix name ( __VA_ARGS__  ) const; \
  prefix operator op ( __VA_ARGS__ ) const
#define _OPERATION( prefix, op, name, ... ) \
  prefix name ( __VA_ARGS__  ); \
  prefix operator op ( __VA_ARGS__ )

namespace JM
{

using real = float;

template<size_t sx, size_t sy, typename T = real>
class Matrix;
template<size_t d, typename T = real>
class Vector;
class Vect2;
class Vect3;
class Vect4;

struct ProjectionData;
struct ProjectionMatrix;


Vector<3> euler2dir( float pitch, float yaw );
Matrix<4,4> DoNothingMatrix();
ProjectionMatrix Projection( ProjectionData d );
ProjectionMatrix Projection_Perspective( ProjectionData d );
ProjectionMatrix Projection_Orthographic( ProjectionData d );
Vect3 Cross( const Vect3& a, const Vect3& b );

float deg2rad( float degs );

template< size_t D, typename T >
class Vector
{
public:
  using M_type = Vector<D,T>;
  static constexpr size_t Dim = D;

public:
  template<typename... N> constexpr Vector( N... vals );
  constexpr Vector( T* vals );
  constexpr Vector( Vect2 v );
  constexpr Vector( Vect3 v );
  constexpr Vector( Vect4 v );

public:
  constexpr T& operator[]( size_t a );
  constexpr T  operator[]( size_t a ) const;

public:
#define TEMPLATE_V template<size_t oD> Vector<D, T>
#define OTH_VECT Vector<oD,T>
  OPERATION( TEMPLATE_V, +, Add, const OTH_VECT& ov );
  OPERATION( TEMPLATE_V, -, Sub, const OTH_VECT& ov );
  OPERATION( TEMPLATE_V, *, Multiply, const OTH_VECT& ov );
  OPERATION( TEMPLATE_V, /, Divide, const OTH_VECT& ov );

#undef TEMPLATE_V
#undef OTH_VECT
public:
  M_type Normalized() const;
  T      Magnitude() const;

public:
  T values[D];
};

class Vect2
{
public:
  constexpr Vect2() {}
  constexpr Vect2( real _x )
  : x( _x ), y( _x ) {}
  constexpr Vect2( real _x, real _y )
  : x( _x ), y( _y ) {}
  constexpr Vect2( Vector<2> v )
  : x( v[0] ), y( v[1] ) {}

public:
  OPERATION( Vect2, +, Add,           const Vect2& v );
  OPERATION( Vect2, -, Sub,           const Vect2& v );
  OPERATION( Vect2, *, Multiply,      const Vect2& v );
  OPERATION( Vect2, /, Divide,        const Vect2& v );

  _OPERATION( Vect2&, +=, PlusEquals,   const Vect2& v );
  _OPERATION( Vect2&, -=, MinusEquals,  const Vect2& v );
  _OPERATION( Vect2&, *=, MultEquals,   const Vect2& v );
  _OPERATION( Vect2&, /=, DivideEquals, const Vect2& v );

public:
  Vect2 Normalized() const;
  real  Magnitude() const;

public:
  union
  {
    struct { real x, y; };
    struct { real u, v; };
  };
};

class Vect3
{
public:
  constexpr Vect3() {}
  constexpr Vect3( real _x )
  : x( _x ), y( _x ), z( _x ) {}
  constexpr Vect3( real _x, real _y )
  : x( _x ), y( _y ), z( _x ) {}
  constexpr Vect3( real _x, real _y, real _z )
  : x( _x ), y( _y ), z( _z ) {}
  constexpr Vect3( Vector<3> v )
  : x( v[0] ), y( v[1] ), z( v[2] ) {}

public:
  OPERATION( Vect3, +, Add,           const Vect3& v );
  OPERATION( Vect3, -, Sub,           const Vect3& v );
  OPERATION( Vect3, *, Multiply,      const Vect3& v );
  OPERATION( Vect3, /, Divide,        const Vect3& v );

  _OPERATION( Vect3&, +=, PlusEquals,   const Vect3& v );
  _OPERATION( Vect3&, -=, MinusEquals,  const Vect3& v );
  _OPERATION( Vect3&, *=, MultEquals,   const Vect3& v );
  _OPERATION( Vect3&, /=, DivideEquals, const Vect3& v );

public:
  Vect3 Normalized() const;
  real  Magnitude() const;

public:
  union
  {
    struct { real x, y, z; };
    struct { real r, g, b; };
    struct { real u, v, w; };
  };
};

class Vect4
{
public:
  constexpr Vect4() {}
  constexpr Vect4( real _x )
  : x( _x ), y( _x ), z( _x ), w( _x ) {}
  constexpr Vect4( real _x, real _y )
  : x( _x ), y( _y ), z( _x ), w( _y ) {}
  constexpr Vect4( real _x, real _y, real _z )
  : x( _x ), y( _y ), z( _z ), w( _x ) {}
  constexpr Vect4( real _x, real _y, real _z, real _w )
  : x( _x ), y( _y ), z( _z ), w( _w ) {}
  constexpr Vect4( Vector<4> v )
  : x( v[0] ), y( v[1] ), z( v[2] ), w( v[3] ) {}


public:
  OPERATION( Vect4, +, Add,           const Vect4& v );
  OPERATION( Vect4, -, Sub,           const Vect4& v );
  OPERATION( Vect4, *, Multiply,      const Vect4& v );
  OPERATION( Vect4, /, Divide,        const Vect4& v );

  _OPERATION( Vect4&, +=, PlusEquals,   const Vect4& v );
  _OPERATION( Vect4&, -=, MinusEquals,  const Vect4& v );
  _OPERATION( Vect4&, *=, MultEquals,   const Vect4& v );
  _OPERATION( Vect4&, /=, DivideEquals, const Vect4& v );

public:
  Vect4 Normalized() const;
  real  Magnitude() const;

public:
  union
  {
    struct { real x, y, z, w; };
    struct { real r, g, b, a; };
  };

};


template < size_t sx, size_t sy, typename T >
class Matrix
{
public:
  static constexpr size_t size_x = sx;
  static constexpr size_t size_y = sy;
  using M_type = Matrix<sx, sy, T>;

public:
  template<typename... N> constexpr Matrix( N... data );

public:
  T& Get( size_t x, size_t y );
  T  Get( size_t x, size_t y ) const;

public:
  T* operator[]( size_t y );

public:
  M_type Transpose() const;

public:
#define V_TEMPLATE template<size_t v> Vector<v> 
#define M_TEMPLATE template<size_t ox, size_t oy, typename ot> Matrix<ox,sy,T> 
#define M_TYPE Matrix<ox,oy,ot>

  OPERATION( V_TEMPLATE, *, Multiply, const Vector<v>& vect );
  OPERATION( M_TEMPLATE, *, Multiply, const M_TYPE& m );
  OPERATION( M_type, *, Multiply, T scaler );
  OPERATION( M_type, +, Add, const M_type& m );
  OPERATION( M_type, -, Sub, const M_type& m );

#undef V_TEMPLATE
#undef M_TEMPLATE
#undef M_TYPE
#undef OPERATION

public:
  T values[sy][sx];

};

struct ProjectionData
{
  bool isPerspective;
  float f, n;
  union {  
    struct { float l, r, t, b; };
    struct { float h, w, fov;  };
  };
};

struct ProjectionMatrix
{
  ProjectionData data;
  Matrix<4,4>    matrix;
};

}

/* in another file for some sense of order */
#include "JMathTemplateDefinitions.cpp"

