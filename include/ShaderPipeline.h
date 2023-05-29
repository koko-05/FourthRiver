#pragma once
#include "Shader.h"

/*
 * Declares a Pipeline that abstracts away shaders into different
 * modules for convienience and so you can use the same code on different
 * objects 
 *
 * */

namespace FourthRiver 
{
namespace Components
{

class ShaderPipeline
    : public Shader
{
public:
    static constexpr uint16_t mId = 2;

    using InitFunc = void(*)(FourthRiver::Object*);
    struct Module
    {
        std::string Name;
        std::string Source;
        uint8_t     Target;
        InitFunc    Initialize;
    };

    enum Shaders { VS = 1, FS = 2 };

public:
    void AddModuleM( uint8_t target, const std::string& name, InitFunc init, const std::string& src );
    template<typename T> void AddModule()  /* static const uint8_t     Target; 
                                              static const std::string Name;
                                              static const std::string Source; 
                                              static void Initialize( Object* ) { YourImpl; } */
    { modules.emplace_back( T::Name, T::Source, T::Target, T::Initialize ); }

    template<typename T> 
    void AddModule( const T& inst )
    { modules.emplace_back( inst.Name, inst.Source, inst.Target, inst.Initialize ); }

public:
    void CreatePipeline();
    void UpdatePipeline();

    void InitializeModules();

public:
    std::vector<Module> modules;

private:
    void ModuleStringFirst( std::string& target, const std::string& name, const std::string& source );

};

}
}

