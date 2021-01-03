#include "precompile.h"
#include "WindowsWindow.h"

#include "Framework/Event/ApplicationEvent.h"
#include "Framework/Event/KeyEvent.h"
#include "Framework/Event/MouseEvent.h"
#include "Framework/Core/Input.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Framework
{
    static bool glfwInitialized = false;

    static void GLFWErrorCallback(int aErrorCode, const char* aErrorDescription)
    {
        GF_CORE_LOG_ERROR("GLFW Error ({0}): {1}", aErrorCode, aErrorDescription);
    }

    Window* Window::Create(const WindowProps& aProps)
    {
        return new WindowsWindow(aProps);
    }

    WindowsWindow::WindowsWindow(const WindowProps& aProps)
    {
        GF_PROFILE_FUNCTION();

        Init(aProps);
    }

    WindowsWindow::~WindowsWindow()
    {
        GF_PROFILE_FUNCTION();

        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& aProps)
    {
        GF_PROFILE_FUNCTION();

        myData.Title = aProps.Title;
        myData.Width = aProps.Width;
        myData.Height = aProps.Height;

        GF_CORE_LOG_INFO("Creating window {0} ({1}, {2})", aProps.Title, aProps.Width, aProps.Height);

        if (!glfwInitialized)
        {
            GF_PROFILE_SCOPE("glfwInit");
            // TODO: glfwTerminate on system shutdown
            int success = glfwInit();
            GF_CORE_ASSERT(success, "Failed to initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
            glfwInitialized = true;
        }

        {
            GF_PROFILE_SCOPE("glfwCreateWindow");
            myWindow = glfwCreateWindow((int)aProps.Width, (int)aProps.Height, myData.Title.c_str(), nullptr, nullptr);
        }
        myContext = CreateScope<OpenGLContext>(myWindow);
        myContext->Init();

        glfwSetWindowUserPointer(myWindow, &myData);
        SetVSync(false);

        glfwSetWindowSizeCallback(myWindow, [](GLFWwindow* aWindow, int aWidth, int aHeight)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(aWindow));
            data.Width = aWidth;
            data.Height = aHeight;

            WindowResizeEvent event(aWidth, aHeight);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(myWindow, [](GLFWwindow* aWindow)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(aWindow));
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(myWindow, [](GLFWwindow* aWindow, int aKey, int aScancode, int aAction, int aMods)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(aWindow));

            switch (aAction)
            {
            case GLFW_PRESS:
            {
                KeyPressedEvent event(static_cast<KeyCode>(aKey), 0);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent event(static_cast<KeyCode>(aKey));
                data.EventCallback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressedEvent event(static_cast<KeyCode>(aKey), 1);
                data.EventCallback(event);
                break;
            }
            }
        });

        glfwSetMouseButtonCallback(myWindow, [](GLFWwindow* aWindow, int aButton, int aAction, int aMods)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(aWindow));

            switch (aAction)
            {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent event(static_cast<MouseCode>(aButton));
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event(static_cast<MouseCode>(aButton));
                data.EventCallback(event);
                break;
            }
            }
        });

        glfwSetScrollCallback(myWindow, [](GLFWwindow* aWindow, double aXOffset, double aYOffset)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(aWindow));

            MouseScrolledEvent event(static_cast<float>(aXOffset), static_cast<float>(aYOffset));
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(myWindow, [](GLFWwindow* aWindow, double aXPosition, double aYPosition)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(aWindow));

            MouseMovedEvent event(static_cast<float>(aXPosition), static_cast<float>(aYPosition));
            data.EventCallback(event);
        });

        glfwSetCharCallback(myWindow, [](GLFWwindow* aWindow, unsigned int aKeyCode)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(aWindow));

            KeyTypedEvent event(static_cast<KeyCode>(aKeyCode));
            data.EventCallback(event);
        });
    }

    void WindowsWindow::Shutdown()
    {
        GF_PROFILE_FUNCTION();

        glfwDestroyWindow(myWindow);
    }

    void WindowsWindow::OnUpdate()
    {
        GF_PROFILE_FUNCTION();

        glfwPollEvents();
        myContext->SwapBuffers();
    }

    void WindowsWindow::SetVSync(bool aEnabled)
    {
        GF_PROFILE_FUNCTION();

        if (aEnabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        myData.VSync = aEnabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return myData.VSync;
    }
}