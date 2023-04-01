
namespace JM
{

/* Vector Template */

template< size_t D, typename T >
template<typename... N> 
constexpr Vector::Vector<D,T>( N... vals )
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

#define TEMPLATE_V template<int od> Vector<D>
#define OTH_VECT
  OPERATION( TEMPLATE_V, +, Add, const OTH_VECT& ov,  );
  OPERATION( TEMPLATE_V, -, Sub, const OTH_VECT& ov );
  OPERATION( TEMPLATE_V, *, Multiply, const OTH_VECT& ov );
  OPERATION( TEMPLATE_V, /, Divide, const OTH_VECT& ov );

#undef TEMPLATE_V
#undef OTH_VECT


}
