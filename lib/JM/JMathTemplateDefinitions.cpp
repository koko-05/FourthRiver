#include <iostream>
#include <cmath>

namespace JM
{

/* Vector Template */

template< size_t D, typename T >
template<typename... N> 
constexpr Vector<D,T>::Vector( N... vals )
  : values{ vals... }
{  }

template< size_t D, typename T >
constexpr Vector<D,T>::Vector( T* vals )
{
  for ( size_t i =0; i < D; i++ )
    values[i] = vals[i];
}

template< size_t D, typename T >
constexpr Vector<D,T>::Vector( Vect2 v )
{
  for ( size_t i =0; i < 2; i++ )
    values[i] = ((T*)(&v.x))[i];
}

template< size_t D, typename T >
constexpr Vector<D,T>::Vector( Vect3 v )
{
  for ( size_t i =0; i < 3; i++ )
    values[i] = ((T*)(&v.x))[i];
}

template< size_t D, typename T >
constexpr Vector<D,T>::Vector( Vect4 v )
{
  for ( size_t i =0; i < 4; i++ )
    values[i] = ((T*)(&v.x))[i];
}

template<size_t D, typename T>
constexpr T& Vector<D,T>::operator[]( size_t a )
{
  return values[a];
}

template<size_t D, typename T>
constexpr T Vector<D,T>::operator[]( size_t a ) const
{
  return values[a];
}

template<size_t D, typename T>
Vector<D,T>::M_type Vector<D,T>::Normalized() const
{
  auto v = Vector<D,T>(Magnitude());
  std::cout <<  v;
  return Divide( v );
}

template<size_t D, typename T>
T Vector<D,T>::Magnitude() const
{
  T acc = 0.0;
  for ( size_t i = 0; i < Dim; i++ )
    acc += values[i] * values[i];

  return std::sqrt( acc );
}

template<size_t D, typename T>
template<size_t oD>
Vector<D,T> Vector<D,T>::Add( const Vector<oD, T>& ov ) const
{
  Vector<D, T> t( values );
  for ( size_t i = 0; i < Vector<oD,T>::Dim && i < Dim; i++ )
    t[i] += ov.values[i];
}

template<size_t D, typename T>
template<size_t oD>
Vector<D,T> Vector<D,T>::operator+( const Vector<oD, T>& ov ) const
{
  return Add( ov );
}

template<size_t D, typename T>
template<size_t oD>
Vector<D,T> Vector<D,T>::Sub( const Vector<oD, T>& ov ) const
{
  Vector<D, T> t( values );
  for ( size_t i = 0; i < Vector<oD,T>::Dim && i < Dim; i++ )
    t[i] -= ov.values[i];
}

template<size_t D, typename T>
template<size_t oD>
Vector<D,T> Vector<D,T>::operator-( const Vector<oD, T>& ov ) const
{
  return Sub( ov );
}

template<size_t D, typename T>
template<size_t oD>
Vector<D,T> Vector<D,T>::Multiply( const Vector<oD, T>& ov ) const
{
  Vector<D, T> t( values );
  for ( size_t i = 0; i < Vector<oD,T>::Dim && i < Dim; i++ )
    t[i] *= ov.values[i];
}

template<size_t D, typename T>
template<size_t oD>
Vector<D,T> Vector<D,T>::operator*( const Vector<oD, T>& ov ) const
{
  return Multiply( ov );
}

template<size_t D, typename T>
template<size_t oD>
Vector<D,T> Vector<D,T>::Divide( const Vector<oD, T>& ov ) const
{
  Vector<D, T> t( values );
  for ( size_t i = 0; i < Vector<oD,T>::Dim && i < Dim; i++ )
    t[i] /= ov.values[i];
}

template<size_t D, typename T>
template<size_t oD>
Vector<D,T> Vector<D,T>::operator/( const Vector<oD, T>& ov ) const
{
  return Divide( ov );
}

/* Matrix */

template < size_t sx, size_t sy, typename T >
template<typename... N> 
constexpr Matrix<sx,sy,T>::Matrix( N... data )
  : values{ data... }
{  }

template < size_t sx, size_t sy, typename T >
T& Matrix<sx,sy,T>::Get( size_t x, size_t y )
{
  return values[y][x];
}

template < size_t sx, size_t sy, typename T >
T  Matrix<sx,sy,T>::Get( size_t x, size_t y ) const
{
  return values[y][x];
}

template < size_t sx, size_t sy, typename T >
T* Matrix<sx,sy,T>::operator[](size_t y)
{
  return values[y];
}

template < size_t sx, size_t sy, typename T >
Matrix<sx,sy,T>::M_type Matrix<sx,sy,T>::Transpose() const
{
  M_type temp;

  for (size_t y = 0; y < sy; y++)
      for ( size_t x = 0; x < sx; x++)
          temp.Get( y, x ) = Get( x, y );
  
  return temp;
}


template < size_t sx, size_t sy, typename T  >
template < size_t vd >
Vector<vd> Matrix<sx,sy,T>::Multiply( const Vector<vd>& v ) const
{
  Matrix<1, vd> matrixVector;
  memcpy( matrixVector[0], v.values, vd * sizeof(T) );
  return Multiply( matrixVector );
}

template < size_t sx, size_t sy, typename T >
template < size_t vd >
Vector<vd> Matrix<sx,sy,T>::operator*( const Vector<vd>& v ) const
{
  return Multiply( v );
}


template < size_t sx, size_t sy, typename T >
template < size_t ox, size_t oy, typename ot >
Matrix<ox,sy,T> Matrix<sx,sy,T>::Multiply( const Matrix<ox,oy,ot>& m ) const
{
  if (sx != oy) return {};

  Matrix<ox, sy> c;

  for (size_t i = 0; i < sy; i++)
      for (size_t j = 0; j < ox; j++)
      {
          T val = 0.0f;
          for ( size_t k = 0; k < oy; k++ )
              val += Get(k, i) * m.Get(j, k);

          c.Get(j,i) = val;
      }
  
  return c;
}

template < size_t sx, size_t sy, typename T >
template < size_t ox, size_t oy, typename ot >
Matrix<ox,sy,T> Matrix<sx,sy,T>::operator*( const Matrix<ox, oy, ot>& m ) const
{
  return Multiply( m );
}

template < size_t sx, size_t sy, typename T >
Matrix<sx,sy,T>::M_type Matrix<sx,sy,T>::Multiply( T scaler ) const
{
  Matrix<sx,sy,T> m;
  for ( size_t i = 0; i < sx * sy; i++ )
    m.values[0][i] = values[0][i] * scaler;
  return m;
}

template < size_t sx, size_t sy, typename T >
Matrix<sx,sy,T>::M_type Matrix<sx,sy,T>::operator*( T scaler ) const
{
  return Multiply( scaler );
}

template < size_t sx, size_t sy, typename T >
Matrix<sx,sy,T>::M_type Matrix<sx,sy,T>::Add( const Matrix<sx,sy,T>& m ) const
{
  Matrix<sx,sy,T> t;
  for ( size_t i = 0; i < sx * sy; i++ )
    t.values[0][i] = values[0][i] + m.values[0][i];
  return m;
}

template < size_t sx, size_t sy, typename T >
Matrix<sx,sy,T>::M_type Matrix<sx,sy,T>::operator+( const Matrix<sx,sy,T>& m ) const
{
  return Add( m );
}

template < size_t sx, size_t sy, typename T >
Matrix<sx,sy,T>::M_type Matrix<sx,sy,T>::Sub( const Matrix<sx,sy,T>& m ) const
{
  Matrix<sx,sy,T> t;
  for ( size_t i = 0; i < sx * sy; i++ )
    t.values[0][i] = values[0][i] - m.values[0][i];
  return m;
}

template < size_t sx, size_t sy, typename T >
Matrix<sx,sy,T>::M_type Matrix<sx,sy,T>::operator-( const Matrix<sx,sy,T>& m ) const
{
  return Sub( m );
}

}
