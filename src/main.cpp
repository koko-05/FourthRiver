#include <iostream>
#include <string>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "JGL/JGL.h"
#include "JM/JMath.h"


JGL::BasicObject CreateRectangle()
{
    // combining all the needed buffers into one object
    JGL::BasicObject ret;
    
    // create buffer on gpu ( this is it goes out of scope, it doesnt matter, bcs its stored on gpu
    // openGL doesnt know that its out of scope ) 
    JGL::VertexBuffer vbo;
    vbo.SetGpuBufferData( GL_STATIC_DRAW,
    {
        /* pos */ -0.5f, -0.5f, /* color */ 1.0f, 0.0f, 0.0f, /* UV */  0.0f, 0.0f,
        /* pos */  0.5f, -0.5f, /* color */ 0.0f, 1.0f, 0.0f, /* UV */  1.0f, 0.0f,
        /* pos */  0.5f,  0.5f, /* color */ 0.0f, 0.0f, 1.0f, /* UV */  1.0f, 1.0f,
        /* pos */ -0.5f,  0.5f, /* color */ 0.5f, 0.5f, 0.5f, /* UV */  0.0f, 1.0f
    } );
    
    // tell OpenGL how to interpret data   
    JGL::VertexAttribute va;
    va.Add( GL_FLOAT, 2 );
    va.Add( GL_FLOAT, 3 );
    va.Add( GL_FLOAT, 2 );
    ret.vao.AddAttrib( vbo, va );

    // Index buffer
    ret.ibo.SetGpuBufferData( GL_STATIC_DRAW, {
        0, 1, 2,
        2, 3, 0
    } );


    // create shader from file
    ret.shader.CreateShaderF( "src/shader.glsl", '~');

    ret.vertexCount = 6;
    
    
    return ret;
}


int main()
{
    // create OpenGL Context and renderer class
    JGL::Renderer render;
    render.CreateWindow( { 800, 800 }, "* __ *" );

    // creates rectangle on GPU
    auto rectangle = CreateRectangle();

    // set mvp
    // PROJECTION
    JM::Projection projType;
    projType.Type = JM::ProjectionType::Orthographic;
    projType.Orthographic.near    = -1.0f;
    projType.Orthographic.far     =  1.0f;
    projType.Orthographic.left    = -1.0f;
    projType.Orthographic.right   =  1.0f;
    projType.Orthographic.bottom  = -1.0f;
    projType.Orthographic.top     =  1.0f;
    JM::Mat<4,4> proj( projType ); 

    // VIEW (translate +0.1 on x)
    JM::Mat<4,4> view( { 
        { 1.0f, 0.0f, 0.0f, 0.1f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f } 
    } );

    // MODEL (translate +0.1 on x)
    JM::Mat<4,4> model( { 
        { 1.0f, 0.0f, 0.0f, 0.1f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f } 
    } );

    JM::Mat<4,4> mvp( proj * view * model );

    // set uniform once and retreive its gl id
    rectangle.shader.Bind();
    GLuint uId     = rectangle.shader.SetUniform4f( "uColor",  { 1.0f, 0.2f, 0.0f, 1.0f } );
    rectangle.shader.SetUniformMat4( "uMVP", mvp );

    // create texture
    JGL::Texture texture( 0 );
    texture.SetImgDataF( "assets/textures/test.png", GL_TEXTURE_2D, GL_RGB, GL_RGBA );
    rectangle.shader.SetUniform1i( "textureId", texture.GetSlot() );

    // ImGui set up
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(render.GetContext().mWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    
    float incr = 0.5f, acc = 0.0f, incrAm = 0.0f;
    JM::Vect4 multColor = { 1.0f };
    while ( ! glfwWindowShouldClose( render.GetContext().mWindow ) )
    {
        render.ClearScreen();
        
        if ( render.GetContext().GetKey( GLFW_KEY_TAB ) == GLFW_PRESS )
            glfwSetWindowShouldClose( render.GetContext().mWindow, GLFW_TRUE );

        if ( acc > 1.0f ) incr = -incrAm;
        if ( acc < 0.0f ) incr =  incrAm;
        acc = acc + incr;
        multColor.w() = std::abs( acc );

        rectangle.shader.SetUniform4f( uId, multColor );

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        render.DrawCall( rectangle, GL_TRIANGLES );    

        // ImGui window
        {
            //create window
            ImGui::Begin("DEBUG WINDOW!");

            ImGui::Text("This is some useful text.");

            ImGui::SliderFloat("float", &incrAm, 0.0f, 0.1f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("MultColor: ", (float*)&multColor); // Edit 3 floats representing a color

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
         
        
        render.EndFrame();
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    return 0;
}
