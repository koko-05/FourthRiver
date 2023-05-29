#include "JGL/Texture.h"
#include "JGL/deps/stb/stb_image.h"

namespace JGL
{

Texture::Texture( uint8_t _bindSlot ) 
    : mIntAttribs( ), mFVecAttribs( )
{
    Activate( _bindSlot );
    glGenTextures( 1, &mId );
    Bind( mSlot );
}

Texture::Texture( const Texture& _o )
    : mIntAttribs( _o.mIntAttribs ), mFVecAttribs( _o.mFVecAttribs )
{
    Activate( _o.mSlot );
    glGenTextures( 1, &mId );
    Bind( mSlot );
    SetImgData( _o.mData, _o.mTarget, _o.mFormat, _o.mDFormat, _o.mWidth, _o.mHeight );
}


Texture& Texture::operator=( Texture& v )
{
    if ( &v == this ) return *this;

    Texture temp( v );

    std::swap( mId, v.mId );
    std::swap( mTarget, v.mTarget );
    std::swap( mFormat, v.mFormat );
    std::swap( mDFormat, v.mDFormat );
    std::swap( mData, v.mData );
    std::swap( mWidth, v.mWidth );
    std::swap( mHeight, v.mHeight );
    std::swap( mSlot, v.mSlot );
    std::swap( mIntAttribs, v.mIntAttribs );
    std::swap( mFVecAttribs, v.mFVecAttribs );

    return *this;
}

Texture::Texture( Texture&& v )
    : mId( v.mId ),
      mTarget( v.mTarget ),
      mFormat( v.mFormat ),
      mDFormat( v.mDFormat ),
      mData( v.mData ),
      mWidth( v.mWidth ),
      mHeight( v.mHeight ),
      mSlot( v.mSlot ),   
      mIntAttribs( std::move(v.mIntAttribs ) ),
      mFVecAttribs( std::move(v.mFVecAttribs) )
{
    v.mId     = 0;
    v.mTarget = 0;
    v.mFormat = 0;
    v.mDFormat= 0;
    v.mData   = nullptr;
    v.mWidth  = 0;
    v.mHeight = 0;
    v.mSlot   = 0;
}

Texture::~Texture()
{
    if ( !mId ) return;

    glDeleteTextures(1, &mId );
    stbi_image_free( (void*)mData );
}

void Texture::SetImgDataF( const char* _filePath, GLenum _target, GLenum _desiredFormat, GLenum _inputFormat )
{
    int width, height, colorChannels;
    stbi_set_flip_vertically_on_load( true );  // flip image
    /* TODO: use colorChannels to create _inputFormat */

    mData = stbi_load(_filePath, &width, &height, &colorChannels, 0); 
    
    ASSERT( mData, "Failed to load image req for texture! (%s)", _filePath);

    SetImgData( mData,_target, _desiredFormat, _inputFormat, width, height );
}



void Texture::SetImgData( const uint8_t* _data, GLenum _target, GLenum _desiredFormat, GLenum _inputFormat, size_t sx, size_t sy )
{
    mData = _data; mHeight = sy; mWidth = sx; mTarget = _target; mFormat = _inputFormat; mDFormat = _desiredFormat;
    if ( _data == nullptr ) return;

    Activate( mSlot );
    if ( mId == 0 ) glGenTextures( 1, &mId );
    Bind( mSlot );

    if ( mIntAttribs.empty() )
        SetParamsToDefault();
    
    for ( const auto& e : mIntAttribs )
        glTexParameteri( _target, e.first, e.second );
    
    for ( const auto& e : mFVecAttribs )
        glTexParameterfv( _target, e.first, e.second );


    glTexImage2D( _target, 0, _desiredFormat, mWidth, mHeight, 0, _inputFormat, GL_UNSIGNED_BYTE, mData );
}


void Texture::SetParameter( GLenum _target, int _val )
{
    mIntAttribs.push_back( { _target, _val } );
}

void Texture::SetParameter( GLenum _target, const float* _val )
{
    mFVecAttribs.push_back( { _target, (float*)_val } );
}

void Texture::ClearParameters()
{
    mIntAttribs.clear();
    mFVecAttribs.clear();
}

void Texture::SetParamsToDefault()
{
    static const float borderColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    SetParameter( GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
    SetParameter( GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
    SetParameter( GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
    SetParameter( GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
    SetParameter( GL_TEXTURE_BORDER_COLOR, borderColor );
    SetParameter( GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    SetParameter( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

void Texture::Bind( uint8_t _slot )
{
    Activate( _slot ); 
    glBindTexture( mTarget, mId );
}

void Texture::Activate( uint8_t _slot )
{
    mSlot = _slot;
    glActiveTexture( GL_TEXTURE0 + _slot );
}


const uint8_t* Texture::GetData() const {
    return mData;
}
uint32_t Texture::GetWidth() const {
    return mWidth;
}
uint32_t Texture::GetHeight() const{
    return mHeight;
}
uint32_t Texture::GetSlot() const {
    return mSlot;
}


}
