#pragma once
#include <vector>
#include "JGL/Shader.h"
#include "JM/JMath.h"
#include "GPUBuffer.h"


/*
 * Interface for managing Uniform Buffer Objects
 *
 * */

namespace JGL
{

class UBO
    : public JGL::GPUBuffer
{
public:
    void Create( size_t index );
    void UpdateBuffer();
    void Associate( JGL::Shader& sh, const char* name);

public:
    void* GetLocation( size_t index );
    void  AddElement( size_t count, size_t align, const char* glslCode);
    void* udata();


    template<typename T>
    T& Get( size_t elementIndex )
    { return *((T*)GetLocation(elementIndex)); }


public:
    struct UniformElement
    {
        size_t offset;
        size_t align;
        const char* glslCode = nullptr;
    };

public:
    template<typename T>
    struct Array 
    { 
        T* arr;
        size_t align;

        T& operator[]( size_t index ) { return *(T*)((char*)arr + align * index); }
    };

public:
    template<typename T>
    Array<T> GetArr( size_t elementIndex )
    { return { GetLocation( elementIndex ), elements[elementIndex].align }; }

public:
    std::vector<UniformElement> elements;
    size_t size = 0;
    int index = -1;

};
    
}
