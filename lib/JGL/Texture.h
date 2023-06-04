#pragma once
#include "common.h"
#include <vector>

namespace JGL
{

/* 
 * Texture abstraction layer for OpenGL JGL library
 *
 * */

class Texture
{
public:
    Texture( uint8_t _bindSlot );
    Texture( const Texture& _o );
    Texture( Texture&& v );
    Texture( );
    ~Texture();

public:
    void SetImgDataF( const char* _filePath, GLenum _target, GLenum _desiredFormat, GLenum _inputFormat = 0);
    void SetImgData( const uint8_t* _data, GLenum _target, GLenum _desiredFormat, GLenum _inputFormat, size_t sx, size_t sy );

public:
    void Activate( uint8_t _slot = 0 );
    void Bind( uint8_t _slot = 0 );

public:
    void SetParameter( GLenum _target, int _val );
    void SetParameter( GLenum _target, const float* _val );
    void ClearParameters();
    void SetParamsToDefault();

public:
    const uint8_t* GetData() const;
    uint32_t GetWidth()      const;
    uint32_t GetHeight()     const;
    uint32_t GetSlot()       const;

public:
    Texture& operator=( Texture& _other );

    GLuint         mId     = 0;
    GLenum         mTarget = 0;
    GLenum         mFormat = 0;
    GLenum         mDFormat= 0;
    const uint8_t* mData   = nullptr;
    uint32_t       mWidth  = 0;
    uint32_t       mHeight = 0;
    uint8_t        mSlot   = 0;

    std::vector< std::pair<GLenum, int> >    mIntAttribs;
    std::vector< std::pair<GLenum, float*> > mFVecAttribs; /* TODO: idk why there is a float* here, it could be dangerous; check this out later */

};

}
