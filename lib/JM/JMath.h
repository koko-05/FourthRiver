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

constexpr Vector<3> Euler2Dir( float pitch, float yaw );
constexpr Matrix<4,4> DoNothingMatrix();
constexpr float deg2rad( float degs );

template< size_t D, typename T = real>
class Vector
{
public:
  using M_type = Vector<D,T>;

public:
  template<T... N> Vector( N... vals );
  template<T... N> constexpr Vector( N... vals );

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
