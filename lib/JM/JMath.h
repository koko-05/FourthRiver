#pragma once

#ifdef _DEBUG
#define JMDEBUG
#endif

#define JM_PI 3.14159265359
#define CLAMP( val, min, max ) ( val > max ? max : ( val < min ? min : val ) )
#define OPERATION( prefix, op, name, ... ) \
  prefix name ( __VA_ARGS__  ) const; \
  prefix operator op ( __VA_ARGS__ ) const;

namespace JM
{

using real = float;

class Vect2;
class Vect3;
class Vect4;

struct ProjectionData
{
  int64_t f, n, l, r, t, b;
};

struct ProjectionMatrix
{
  bool           isPerspective;
  ProjectionData data;
  Matrix<4,4>    matrix;
};

constexpr Vector<3> euler2dir( float pitch, float yaw );
constexpr Matrix<4,4> DoNothingMatrix()
constexpr ProjectionMatrix Projection_Perspective( ProjectionData d );
constexpr ProjectionMatrix Projection_Orthographic( ProjectionData d );
constexpr Cross( const Vect3& a, const Vect3& b );

constexpr float deg2rad( float degs );

template< size_t D, typename T = real>
class Vector
{
public:
  using M_type = Vector<D,T>;

public:
  template<typename... N> constexpr Vector( N... vals );
  constexpr Vector( T* vals );
  constexpr Vector( Vect2 v );
  constexpr Vector( Vect3 v );
  constexpr Vector( Vect4 v );

public:
#define TEMPLATE_V template<int od> Vector<D>
#define OTH_VECT
  OPERATION( TEMPLATE_V, +, Add, const OTH_VECT& ov );
  OPERATION( TEMPLATE_V, -, Sub, const OTH_VECT& ov );
  OPERATION( TEMPLATE_V, *, Multiply, const OTH_VECT& ov );
  OPERATION( TEMPLATE_V, /, Divide, const OTH_VECT& ov );

#undef TEMPLATE_V
#undef OTH_VECT
public:
  M_type Normalized() const;
  T      Magnitude() const;
  M_type Rotate( const float deg, M_type pivot ) const;
  

public:
  T values[D];
};

class Vect2
{
public:
  constexpr Vect2( real x );
  constexpr Vect2( real x, real y );
  constexpr Vect2( Vector<2> v );

public:
  OPERATION( Vect2, +, Add,           const Vect2& v );
  OPERATION( Vect2, -, Sub,           const Vect2& v );
  OPERATION( Vect2, *, Multiply,      const Vect2& v );
  OPERATION( Vect2, /, Divide,        const Vect2& v );

  OPERATION( Vect2, +=, PlusEquals,   const Vect2& v );
  OPERATION( Vect2, -=, MinusEquals,  const Vect2& v );
  OPERATION( Vect2, *=, MultEquals,   const Vect2& v );
  OPERATION( Vect2, /=, DivideEquals, const Vect2& v );

public:
  union
  {
    real x, y;
    real u, v;
  };
};

class Vect3
{
public:
  constexpr Vect3( real x );
  constexpr Vect3( real x, real y );
  constexpr Vect3( real x, real y, real z );
  constexpr Vect3( Vector<4> v );

public:
  OPERATION( Vect3, +, Add,           const Vect3& v );
  OPERATION( Vect3, -, Sub,           const Vect3& v );
  OPERATION( Vect3, *, Multiply,      const Vect3& v );
  OPERATION( Vect3, /, Divide,        const Vect3& v );

  OPERATION( Vect3, +=, PlusEquals,   const Vect3& v );
  OPERATION( Vect3, -=, MinusEquals,  const Vect3& v );
  OPERATION( Vect3, *=, MultEquals,   const Vect3& v );
  OPERATION( Vect3, /=, DivideEquals, const Vect3& v );

public:
  union
  {
    real x, y, z;
    real r, g, b;
    real u, v, w;
  };
};

class Vect4
{
public:
  constexpr Vect4( real x );
  constexpr Vect4( real x, real y );
  constexpr Vect4( real x, real y, real z );
  constexpr Vect4( real x, real y, real z, real w );
  constexpr Vect4( Vector<4> v )

public:
  OPERATION( Vect4, +, Add,           const Vect4& v );
  OPERATION( Vect4, -, Sub,           const Vect4& v );
  OPERATION( Vect4, *, Multiply,      const Vect4& v );
  OPERATION( Vect4, /, Divide,        const Vect4& v );

  OPERATION( Vect4, +=, PlusEquals,   const Vect4& v );
  OPERATION( Vect4, -=, MinusEquals,  const Vect4& v );
  OPERATION( Vect4, *=, MultEquals,   const Vect4& v );
  OPERATION( Vect4, /=, DivideEquals, const Vect4& v );

public:
  union
  {
    real x, y, z, w;
    real r, g, b, a;
  };

};


template < size_t sx, size_t sy, typename T = real>
class Matrix
{
public:
  static constexpr size_t size_x = sx;
  static constexpr size_t size_y = sy;
  using M_type = Matrix<sx, sy, T>;

public:
  template<T... N> Matrix( N... data );
  template<T... N> constexpr Matrix( N... data );

public:
  T& Get( size_t x, size_t y );
  T  Get( size_t x, size_t y ) const;

public:
  M_type Transpose() const;

public:
#define V_TEMPLATE template<size_t v> constexpr Vector<v> 
#define M_TEMPLATE template<size_t ox, size_t oy, typename ot> constexpr M_type 
#define M_TYPE Matrix<ox,oy,ot>

  OPERATION( V_TEMPLATE,       *, Multiply, const Vector<v>& v );
  OPERATION( M_TEMPLATE,       *, Multiply, const M_TYPE& m );
  OPERATION( constexpr M_type, *, Multiply,       T scaler );
  OPERATION( constexpr M_type, +, Add,      const M_type& m );
  OPERATION( constexpr M_type, -, Sub,      const M_type& m );

#undef V_TEMPLATE
#undef M_TEMPLATE
#undef M_TYPE
#undef OPERATION

public:
  T values[sy][sx];

};

}

/* in another file for some sense of order */
#include "JMathTemplateDefinitions.cpp"

