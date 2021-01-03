#pragma once
#include "Framework/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Framework
{
    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* aWindowHandle);
        ~OpenGLContext();

        virtual void Init() override;
        virtual void SwapBuffers() override;
    private:
        GLFWwindow* myWindowHandle;
    };
}