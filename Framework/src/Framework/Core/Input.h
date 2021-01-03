#pragma once
#include "Framework/Core/Core.h"
#include "Framework/Core/KeyCodes.h"
#include "Framework/Core/MouseCodes.h"

namespace Framework
{
    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode aKeyCode);

        static bool IsMouseButtonPressed(MouseCode aButton);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };
}