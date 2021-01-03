#include "precompile.h"
#include "OpenGLContext.h"

#include <GLFW\glfw3.h>
#include <glad/glad.h>

namespace Framework
{
    OpenGLContext::OpenGLContext(GLFWwindow* aWindowHandle)
        : myWindowHandle(aWindowHandle)
    {
        GF_CORE_ASSERT(aWindowHandle, "Window handle is null!");
    }

    OpenGLContext::~OpenGLContext()
    {
    }

    void OpenGLContext::Init()
    {
        GF_PROFILE_FUNCTION();

        glfwMakeContextCurrent(myWindowHandle);

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        GF_CORE_ASSERT(status, "Failed to initialize Glad!");

        GF_CORE_LOG_INFO("OpenGL Info:");
        GF_CORE_LOG_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        GF_CORE_LOG_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        GF_CORE_LOG_INFO("  Version: {0}", glGetString(GL_VERSION));

#ifdef GF_ENABLE_ASSERTS
        int versionMajor;
        int versionMinor;
        glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
        glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

        GF_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Framework requires at least OpenGL version 4.5!");
#endif
    }

    void OpenGLContext::SwapBuffers()
    {
        GF_PROFILE_FUNCTION();

        glfwSwapBuffers(myWindowHandle);
    }
}