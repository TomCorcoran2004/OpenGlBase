#include "Base.h"
#include "GLFW/glfw3.h"

namespace Base
{
    void Init()
    {
        glfwInit();
    }

    void Destroy()
    {
        glfwTerminate();
    }
};