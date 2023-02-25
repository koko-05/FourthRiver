#pragma once
#include <memory>
#include <string.h>
#include <iostream>
#include <utility>
#include <iostream>
#include <array>
#include <cmath>

#ifdef _DEBUG
  #define JMDEBUG
#endif

#define JM_PI 3.14159265359
#define CLAMP( val, min, max ) ( val > max ? max : ( val < min ? min : val ) )

/*
 * TODO: add move constructor and move assigment
 * operator overload
 *
 * Add constexpr constructors and functios for projection and the DoNothingMatrix
 *
 * rework everything in order to not use function to access the vars in vector
 * */

namespace JM 
{

using real = float;

enum ProjectionType
{
    Orthographic, Perspective  
};

struct Projection {
public:
    Projection() {}
    Projection( ProjectionType t, int w, int h );

public:
    ProjectionType Type;
    
    union {
        struct {
            real near, far, left, right, top, bottom;
        } Orthographic;
        struct {
            real far, near, height, width, fov;
        } Perspective;
    };
};

/* prototypes */
template<int xOrder, int yOrder> class Mat;
template <int dims> class Vect;
class Vect2;
class Vect3;
class Vect4;

/* Usefull functions */
Vect3 EulerAnglesToDirection( float pitch, float yaw );

Mat<4,4> DoNothingMatrix();

constexpr float degToRad( float degs )
{
    return degs * ( JM_PI / 180.0f );
}



/* Vector template generic: common functions, uses real ( default float ) */
template< int dims >
class Vect 
{
public:
    Vect( std::initializer_list<real> _il );
    Vect( const Mat<1, dims>& _mV );
    Vect();

public:
    template< int othDim >
    Vect<dims> Add(const Vect<othDim>& _other ) const; 

    template< int othDim >
    Vect<dims> Sub(const Vect<othDim>& _other ) const; 

    template< int othDim >
    Vect<dims> Mult(const Vect<othDim>& _other ) const; 

    template< int othDim >
    Vect<dims> Div(const Vect<othDim>& _other ) const; 

public:
    Vect<dims> Normalized() const;
    real Magnitude() const;
    Vect<dims> Rotate( const float _deg, const Vect<dims> _pivot );

public:
    template< int othDim > 
    Vect<dims> operator+(const Vect<othDim>& _other ) const; 

    template< int othDim >
    Vect<dims> operator-(const Vect<othDim>& _other ) const; 

    template< int othDim >
    Vect<dims> operator*(const Vect<othDim>& _other ) const; 

    Vect<dims> operator*(float val) const; 

    template< int othDim >
    Vect<dims> operator/(const Vect<othDim>& _other ) const;

public:
    real mVals[dims];
};


template< int dims >
real Dot( const Vect<dims>& _a, const Vect<dims>& _b);

Vect<3> Cross( const Vect<3>& _a, const Vect<3>& _b);


/* Vect Specialization: 2 float vector */ 
struct Vect2 : public Vect<2> 
{
    Vect2( const Vect<2>& _d ) : Vect<2>( _d ) {}
    Vect2( float x ) : Vect<2>( { x, x } ) {}
    Vect2( float x, float y ) : Vect<2>( { x, y } ) {}

    Vect2()  : Vect<2>() {}

    real& x() { return mVals[0]; }
    real& y() { return mVals[1]; }
    const real& x() const { return mVals[0]; }
    const real& y() const { return mVals[1]; }
};


/* Vect Specialization: 3 float vector */ 
struct Vect3 : public Vect<3> 
{
    Vect3( const Vect<3>& _d ) : Vect<3>( _d ) {}
    Vect3( float x ) : Vect<3>( { x, x, x } ) {}
    Vect3( float x, float y ) : Vect<3>( { x, y, x } ) {}
    Vect3( float x, float y, float z ) : Vect<3>( { x, y, z } ) {}

    Vect3()  : Vect<3>() {}

    real& x() { return mVals[0]; }     
    real& y() { return mVals[1]; }     
    real& z() { return mVals[2]; }     
    const real& x() const { return mVals[0]; }     
    const real& y() const { return mVals[1]; }     
    const real& z() const { return mVals[2]; }     

    real& r() { return mVals[0]; }     
    real& g() { return mVals[1]; }     
    real& b() { return mVals[2]; }     
    const real& r() const { return mVals[0]; }     
    const real& g() const { return mVals[1]; }     
    const real& b() const { return mVals[2]; }     
};


/* Vect Specialization: 4 float vector */ 
struct Vect4 : public Vect<4> 
{
    Vect4( const Vect<4>& _d ) : Vect<4>( _d ) {}
    Vect4( float x ) : Vect<4>( { x, x, x, x } ) {}
    Vect4( float x, float y ) : Vect<4>( { x, y, x, y } ) {}
    Vect4( float x, float y, float z ) : Vect<4>( { x, y, z, x } ) {}
    Vect4( float x, float y, float z, float w ) : Vect<4>( { x, y, z, w } ) {}

    Vect4()  : Vect<4>() {}

    real& x() { return mVals[0]; }     
    real& y() { return mVals[1]; }     
    real& z() { return mVals[2]; }     
    real& w() { return mVals[3]; }     
    const real& x() const { return mVals[0]; }     
    const real& y() const { return mVals[1]; }     
    const real& z() const { return mVals[2]; }     
    const real& w() const { return mVals[3]; }     

    real& r() { return mVals[0]; }     
    real& g() { return mVals[1]; }     
    real& b() { return mVals[2]; }     
    real& a() { return mVals[3]; }     
    const real& r() const { return mVals[0]; }     
    const real& g() const { return mVals[1]; }     
    const real& b() const { return mVals[2]; }     
    const real& a() const { return mVals[3]; }     

};

/* Matrix template class, int, int uses reals (default float) */
template<int xOrder, int yOrder>
class Mat 
{
public:
    using _T  = Mat<xOrder, yOrder>;

public:
    Mat( const std::initializer_list<std::initializer_list<real>>& _il );
    Mat( Projection _Proj ); 
    Mat( const _T& v );
    _T& operator=( const _T&  v );
    _T& operator=(       _T&& v );
    Mat() {}

public:
    template<int vDims>
    Vect<yOrder> Mult( const Vect<vDims>& _v ) const;

    template< int xOrd, int yOrd >
    Mat<xOrd, yOrder> Mult( const Mat<xOrd, yOrd>& _m) const;

    _T Mult( const real _scaler) const;
    _T Add ( const _T& _m) const;
    _T Sub ( const _T& _m) const;

public:
    template<int xOrd, int yOrd>
    Mat<xOrd, yOrder> operator*( const Mat<xOrd, yOrd>& _m ) const;

    template<int vDims>
    Vect<yOrder> operator*( const Vect<vDims>& _v ) const;

    _T operator*( const real _scaler ) const;

    _T operator+( const _T& _m) const;

    _T operator-( const _T& _m) const;

    real& operator()( int x, int y ) { return Get(x, y); }

public:
    Mat<yOrder,xOrder> Transpose() const;
    
public:
    real& Get( size_t _x, size_t y );
    real  Getc( size_t _x, size_t _y ) const;

public:
    real mVals[yOrder][xOrder];
    const uint32_t xSize = xOrder, ySize = yOrder;
};


}





/* Template definitions */

/* std cout support */
template < int dims >
std::ostream& operator<<( std::ostream& _outF, const JM::Vect<dims>& _d )
{
    std::cout << "{ ";
    for ( int i = 0; i < dims; i++ )
    {
        std::cout << _d.mVals[i];
        if ( i == dims - 1 ) break;
        std::cout << ", ";
    }
    std::cout << " }";
    
    return _outF;
}

template < int xOrder, int yOrder >
std::ostream& operator<<( std::ostream& _outF, const JM::Mat<xOrder,yOrder>& _d )
{
    std::cout << "[";
    for ( int y = 0; y < yOrder; y++ )
    {
        std::cout << "\n  { ";
        for ( int x = 0; x < xOrder; x++ )
        {
            std::cout << _d.Getc(x,y);
            if ( x == xOrder-1 ) break;
            std::cout << ", ";
        }
        std::cout << "}, ";
    }
    std::cout << "\n]";
    return _outF;
}

/* Vect generic */
namespace JM {

    template<int dims>
    Vect<dims>::Vect( const Mat<1, dims>& _mV )
    {
        for ( int i = 0; i < dims; i++ )
        {
            mVals[i] = _mV.Getc( 0, i );
        }
    }

    template<int dims>
    Vect<dims>::Vect(std::initializer_list<real> _il)
    {
        #ifdef JMDEBUG
            if ( _il.size() > dims ) printf( "Tried to create a vector with more elements than can fit inside it" );
        #endif 

        auto e = _il.begin();
        for ( int i = 0; i < dims; i++ )
            mVals[i] = e[i % _il.size()];
    }

    template<int dims>
    Vect<dims>::Vect()
    {
        for( int i = 0; i < dims; i++ )
            mVals[i] = 0.0f;
    }

    template<int dims> 
    template<int othDim>
    Vect<dims> Vect<dims>::Add(const Vect<othDim>& _other) const
    {
        Vect<dims> temp(*this);

        for ( int i = 0; i < dims; i++ )
            temp.mVals[i] += _other.mVals[i % othDim];

        return temp;
    }

    template<int dims> 
    template<int othDim>
    Vect<dims> Vect<dims>::Sub( const Vect<othDim>& _other) const
    {
        Vect<dims> temp(*this);

        for ( int i = 0; i < dims; i++ )
            temp.mVals[i] -= _other.mVals[i % othDim];

        return temp;
    }

    template<int dims> 
    template<int othDim>
    Vect<dims> Vect<dims>::Mult( const Vect<othDim>& _other) const
    {
        Vect<dims> temp(*this);

        for ( int i = 0; i < dims; i++ )
            temp.mVals[i] *= _other.mVals[i % othDim];

        return temp;
    }

    template<int dims> 
    template<int othDim>
    Vect<dims> Vect<dims>::Div( const Vect<othDim>& _other) const
    {
        Vect<dims> temp(*this);

        for ( int i = 0; i < dims; i++ )
            temp.mVals[i] /= _other.mVals[i % othDim];

        return temp;
    }
    
    template<int dims>
    real Vect<dims>::Magnitude() const
    {
        return std::sqrt( Dot( *this, *this ) );
    }

    template<int dims>
    Vect<dims> Vect<dims>::Normalized() const
    {
        real mag = Magnitude();
        Vect<dims> temp = *this;
        for ( int i = 0; i < dims; i++ )
            temp.mVals[i] /= mag;

        return temp;
    }

    template<int dims>
    Vect<dims> Vect<dims>::Rotate( const float _deg, const Vect<dims> _pivot )
    {
    }

    template<int dims>
    template<int othDim>
    Vect<dims> Vect<dims>::operator+( const Vect<othDim>& _other) const
    {
        return Add( _other );
    }

    template<int dims>
    template<int othDim>
    Vect<dims> Vect<dims>::operator-( const Vect<othDim>& _other) const
    {
        return Sub( _other );
    }

    template<int dims>
    template<int othDim>
    Vect<dims> Vect<dims>::operator*( const Vect<othDim>& _other) const
    {
        return Mult( _other );
    }

    template<int dims>
    Vect<dims> Vect<dims>::operator*(float val) const
    {
        return Mult( Vect<dims>( { val } ) );
    }

    template<int dims>
    template<int othDim>
    Vect<dims> Vect<dims>::operator/( const Vect<othDim>& _other) const
    {
        return Div( _other );
    }

    template< int dims >
    real Dot( const Vect<dims>& _a, const Vect<dims>& _b ) {

        real acc = 0.0f;
        for ( int i = 0; i < dims; i++ )
            acc += _a.mVals[i] * _b.mVals[i];

        return acc;
    }
}

/* Matrix template definitions */
namespace JM {

    template<int xOrder, int yOrder>
    Mat<xOrder,yOrder>::Mat( const std::initializer_list<std::initializer_list<real>>&  _l ) 
    {
        for ( int i = 0; i < yOrder; i++)
            for ( int j = 0; j < xOrder; j++)
            {
                Get( j, i ) = *((_l.begin() +i)->begin() + j);
            }
    }

    template<int xOrder, int yOrder>
    Mat<xOrder,yOrder>::Mat( Projection _Proj ) 
    {
        if ( xOrder != 4 || yOrder != 4 ) 
        {
            std::cout << "Can only construct a Projection matrix with a 4x4 Matrix\n";
            return;
        }

        if ( _Proj.Type == ProjectionType::Orthographic )
        {
            auto& o = _Proj.Orthographic;

            // x
            Get( 0, 0 ) = 2.0f / (o.right - o.left);
            Get( 0, 1 ) = 0;
            Get( 0, 2 ) = 0;
            Get( 0, 3 ) = 0;

            // y
            Get( 1, 0 ) = 0;
            Get( 1, 1 ) = 2.0f / ( o.top - o.bottom );
            Get( 1, 2 ) = 0;
            Get( 1, 3 ) = 0; 

            // z
            Get( 2, 0 ) = 0;
            Get( 2, 1 ) = 0;
            Get( 2, 2 ) = -2.0f / ( o.near - o.far);
            Get( 2, 3 ) = 0;

            // w
            Get( 3, 0 ) = -(o.right + o.left) / (o.right - o.left);
            Get( 3, 1 ) = -(o.top + o.bottom) / ( o.top - o.bottom);
            Get( 3, 2 ) = -(o.far + o.near) / ( o.far - o.near );
            Get( 3, 3 ) = 1;
        }
        else if ( _Proj.Type == ProjectionType::Perspective )
        {
            auto& p = _Proj.Perspective;
            Mat<4,4> m =
            {
                { 1.0f / ((std::tan( p.fov * 0.5f ) * (p.width / p.height) )), 0.0f, 0.0f, 0.0f },
                { 0.0f, 1.0f / ( std::tan(p.fov * 0.5f) ), 0.0f, 0.0f },
                { 0.0f, 0.0f, (p.near + p.far ) / ( p.near - p.far ), ( 2.0f * p.near * p.far ) / ( p.near - p.far ) },
                { 0.0f, 0.0f, -1.0f, 0.0f }
            };

            *this = m;
        }
    }

    template<int xOrder, int yOrder>
    Mat<xOrder,yOrder>::Mat( const _T& v )
    {
        memcpy( *mVals, *v.mVals, sizeof( real ) * xOrder * yOrder );
    }

    template<int xOrder, int yOrder>
    Mat<xOrder,yOrder>& Mat<xOrder,yOrder>::operator=( const _T&  v )
    {
        memcpy( *mVals, *v.mVals, sizeof( real ) * xOrder * yOrder );
        return *this;
    }
    template<int xOrder, int yOrder>
    Mat<xOrder,yOrder>& Mat<xOrder,yOrder>::operator=( _T&&  v )
    {
        memcpy( *mVals, *v.mVals, sizeof( real ) * xOrder * yOrder );
        return *this;
    }

    template<int xOrder, int yOrder>
    template<int vDims>
    Vect<yOrder> Mat<xOrder,yOrder>::Mult( const Vect<vDims>& _v ) const
    {
        Mat<1, vDims> matrixVector;
        memcpy( matrixVector.mVals[0], _v.mVals, vDims * sizeof(real) );
        return Mult( matrixVector );
    }

    template<int xOrder, int yOrder>
    template<int xOrd, int yOrd>
    Mat<xOrd,yOrder> Mat<xOrder,yOrder>::Mult( const Mat<xOrd,yOrd>& _m) const
    {
        if (xOrder != yOrd) return {};

        Mat<xOrd, yOrder> c;

        for (int i = 0; i < yOrder; i++)
            for (int j = 0; j < xOrd; j++)
            {
                real val = 0.0f;
                for ( int k = 0; k < yOrd; k++ )
                    val += Getc(k, i) * _m.Getc(j, k);

                c.Get(j,i) = val;
            }
        
        return c;
    }


    template<int xOrder, int yOrder>
    real& Mat<xOrder, yOrder>::Get( size_t _x, size_t _y )
    {
        return mVals[_y][_x];
    }

    template<int xOrder, int yOrder>
    real Mat<xOrder, yOrder>::Getc( size_t _x, size_t _y ) const
    {
        return mVals[_y][_x];
    }
    
    template<int xOrder, int yOrder>
    Mat<xOrder,yOrder> Mat<xOrder,yOrder>::Mult( const real _scaler) const
    {
        Mat<xOrder, yOrder> t;
        for ( int i = 0; i < xOrder * yOrder; i++ )
            t.mVals[i] *= _scaler;
        
        return t;
    } 

    template<int xOrder, int yOrder>
    Mat<xOrder,yOrder> Mat<xOrder,yOrder>::Add( const Mat<xOrder,yOrder>& _oth) const
    {
        Mat<xOrder, yOrder> t;
        
        for ( int i = 0; i < xOrder * yOrder; i++ )
            t.mVals[i] += _oth.mVals[i];
        
        return t;
    } 

    template<int xOrder, int yOrder>
    Mat<xOrder,yOrder> Mat<xOrder,yOrder>::Sub( const Mat<xOrder,yOrder>& _oth) const
    {
        Mat<xOrder, yOrder> t;
        
        for ( int i = 0; i < xOrder * yOrder; i++ )
            t.mVals[i] -= _oth.mVals[i];
        
        return t;
    } 

    template<int xOrder, int yOrder>
    template<int xOrd, int yOrd>
    Mat<xOrd, yOrder> Mat<xOrder, yOrder>::operator*( const Mat<xOrd, yOrd>& _m ) const
    {
        return Mult( _m );
    }

    template<int xOrder, int yOrder>
    template<int vDims>
    Vect<yOrder> Mat<xOrder, yOrder>::operator*( const Vect<vDims>& _v ) const
    {
        return Mult( _v );
    }

    template<int xOrder, int yOrder>
    Mat<xOrder,yOrder> Mat<xOrder, yOrder>::operator*( const real _scaler ) const
    {
        return Mult(_scaler);
    }

    template<int xOrder, int yOrder>
    Mat<xOrder,yOrder> Mat<xOrder, yOrder>::operator+( const _T& _m) const
    {
        return Add( _m );
    }

    template<int xOrder, int yOrder>
    Mat<xOrder, yOrder> Mat<xOrder, yOrder>::operator-( const _T& _m) const
    {
        return Sub( _m );
    }

    template<int xOrder, int yOrder>
    Mat<yOrder, xOrder> Mat<xOrder, yOrder>::Transpose() const
    {
        Mat<yOrder,xOrder> temp;

        for (int y = 0; y < yOrder; y++)
            for ( int x = 0; x < xOrder; x++)
                temp( y, x ) = Getc( x, y );
        
        return temp;
    }
}
