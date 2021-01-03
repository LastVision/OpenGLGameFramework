#pragma once
#include "Framework/Core/Core.h"

#include "Framework/Core/Window.h"
#include "Framework/Core/LayerStack.h"
#include "Framework/Event/Event.h"
#include "Framework/Event/ApplicationEvent.h"
#include "Framework/Core/Timestep.h"

#include "Framework/ImGui/ImGuiLayer.h"

namespace Framework
{
    class Application
    {
    public:
        Application(const std::string& name = "LV Application");
        virtual ~Application();

        void Run();
        void Shutdown();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        void OnEvent(Event& event);

        inline Window& GetWindow() const { return *myWindow; }

        inline ImGuiLayer* GetImGuiLayer() { return myImGuiLayer; }

        inline static Application& Get() { return *ourInstance; }

    private:
        bool OnWindowClose(WindowCloseEvent& event);
        bool OnWindowResize(WindowResizeEvent& event);
    private:
        LayerStack myLayerstack;

        Framework::Scope<Window> myWindow;
        ImGuiLayer* myImGuiLayer;
        bool myRunningState, myMinimized;
        float myLastFrameTime = 0.0f;
    private:
        static Application* ourInstance;
    };

    //To be defined in client
    Application* CreateApplication();
}