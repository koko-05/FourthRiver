#pragma once
#include "JM/JMath.h"
#include "GPUBuffer.h"


/*
 * Interface for managing Uniform Buffer Objects
 *
 * */

namespace JGL
{

class UBO
    : protected JGL::GPUBuffer
{
public:
    void Create( size_t index );
    void UpdateBuffer();

public:
    void* GetLocation( size_t index );
    void  AddElement( size_t count, size_t align, const char* glslCode);
    void* udata();


    template<typename T>
    T& Get( size_t elementIndex )
    { return *((T*)GetLocation(elementIndex)); }

    template<typename T>
    UniformArray<T>& Get<UniformArray<T>>( size_t elementIndex )
    { return { GetLocation(elementIndex), elements.align }; }

public:
    struct UniformElement
    {
        size_t offset;
        size_t align;
        const char* glslCode;
    }

public:
    template<typename T>
    struct UniformArray 
    { 
        T* arr;
        size_t align;

        operator[]( size_t index ) { return *(T*)((char*)arr + align * index); }
    };

public:
    std::vector<UniformElement> elements;
    size_t size = 0;

};
    
}
