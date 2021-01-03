#pragma once

#include "Framework/Core/Window.h"
#include "Framework/Renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>

namespace Framework
{
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return myData.Width; }
        inline unsigned int GetHeight() const override { return myData.Height; }

        // Window attributes
        inline void SetEventCallback(const EventCallbackFn& callback) override { myData.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        inline virtual void* GetNativeWindow() const { return myWindow; }
    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();
    private:
        GLFWwindow* myWindow;
        Scope<GraphicsContext> myContext;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData myData;
    };
}