#include <OpenGlBase/Base.h>
#include <OpenGlBase/Window/Window.h>
#include <OpenGlBase/Input/Input.h>
#include <OpenGLBase/Debug/Log.h>
#include <OpenGLBase/Render/Render2D.h>

int main()
{
    Base::Init();
    
    Base::Window::Config Config{
        .Size = glm::ivec2{800, 600},
    };
    Base::Window::Init(Config);
    
    Base::Input::Init();
    Base::Render2D::Init();

    while (Base::Window::ShouldClose() == false)
    {
        Base::Window::Tick();
        Base::Input::Tick();

        // Triangles
        Base::Render2D::AddTriangle(
            vec2(100.0f, 100.0f),   // Point 1
            vec2(200.0f, 100.0f),   // Point 2
            vec2(150.0f, 200.0f),   // Point 3
            vec4(1.0f, 0.0f, 0.0f, 1.0f)  // Red color
        );

        // Quads
        Base::Render2D::AddQuad(
            vec2(300.0f, 100.0f),   // Point 1
            vec2(400.0f, 100.0f),   // Point 2
            vec2(300.0f, 200.0f),   // Point 3
            vec2(400.0f, 200.0f),   // Point 4
            vec4(0.0f, 1.0f, 0.0f, 1.0f)  // Green color
        );

        // Lines
        Base::Render2D::AddLine(
            vec2(100.0f, 300.0f),   // Start
            vec2(200.0f, 350.0f),   // End
            3.0f,                    // Thickness in pixels
            vec4(0.0f, 0.0f, 1.0f, 1.0f)  // Blue color
        );

        // Arrows
        Base::Render2D::AddArrow(
            vec2(300.0f, 300.0f),   // Start
            vec2(400.0f, 350.0f),   // End
            4.0f,                    // Thickness
            vec4(1.0f, 1.0f, 0.0f, 1.0f)  // Yellow color
        );

        // Points
        Base::Render2D::AddPoint(
            vec2(500.0f, 150.0f),   // Position
            10.0f,                   // Size in pixels
            vec4(1.0f, 0.0f, 1.0f, 1.0f)  // Magenta color
        );

        Base::Render2D::Render();

    }

    Base::Render2D::Destroy();
    Base::Input::Destroy();
    Base::Window::Destroy();
    Base::Destroy();
    return 0;
}
