#include "precompile.h"
#include "Framework/Core/Input.h"

#include "Framework/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Framework
{
    bool Input::IsKeyPressed(KeyCode aKeyCode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, static_cast<int32_t>(aKeyCode));
        return (state == GLFW_PRESS || state == GLFW_REPEAT);
    }

    bool Input::IsMouseButtonPressed(MouseCode aButton)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, static_cast<int32_t>(aButton));
        return (state == GLFW_PRESS);
    }

    std::pair<float, float> Input::GetMousePosition()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return { static_cast<float>(xPos), static_cast<float>(yPos) };
    }

    float Input::GetMouseX()
    {
        auto [x, y] = GetMousePosition();
        return x;
    }

    float Input::GetMouseY()
    {
        auto [x, y] = GetMousePosition();
        return y;
    }
}