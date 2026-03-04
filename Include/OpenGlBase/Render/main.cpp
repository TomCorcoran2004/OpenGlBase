#include <OpenGlBase/Base.h>
#include <OpenGlBase/Window/Window.h>
#include <OpenGlBase/Input/Input.h>
#include <OpenGLBase/Debug/Log.h>


int main()
{
    Base::Init();
    Base::Window::Config Config{
        .Size = glm::ivec2{800, 600},
    };
    Base::Window::Init(Config);
    Base::Input::Init();

    while (Base::Window::ShouldClose() == false)
    {
        Base::Window::Tick();
        Base::Input::Tick();

    }


    Base::Destroy();
    return 0;
}
