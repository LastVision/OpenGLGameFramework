#include "precompile.h"
#include "Application.h"

#include "Framework/Renderer/Renderer.h"

#include "Framework/Core/Input.h"

#include <GLFW/glfw3.h>

namespace Framework
{
    Application* Application::ourInstance = nullptr;

    Application::Application(const std::string& aName)
    {
        GF_CORE_ASSERT(!ourInstance, "Application already exists!");
        ourInstance = this;

        myWindow = Framework::Scope<Window>(Window::Create(WindowProps(aName)));
        myWindow->SetEventCallback(GF_BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

        myImGuiLayer = new ImGuiLayer();
        PushOverlay(myImGuiLayer);

        myRunningState = true;
        myMinimized = false;
    }

    Application::~Application()
    {
        //Renderer::Shutdown();
    }

    void Application::Run()
    {
        while (myRunningState)
        {
            float time = static_cast<float>(glfwGetTime());
            Timestep timestep = time - myLastFrameTime;
            myLastFrameTime = time;

            if (myMinimized == false)
            {
                {
                    for (auto* layer : myLayerstack)
                    {
                        layer->OnUpdate(timestep);
                    }
                }


                myImGuiLayer->Begin();
                {
                    for (auto* layer : myLayerstack)
                    {
                        layer->OnImGuiRender();
                    }
                }
                myImGuiLayer->End();
            }

            myWindow->OnUpdate();
        }
    }

    void Application::Shutdown()
    {
        myRunningState = false;
    }

    void Application::PushLayer(Layer* aLayer)
    {
        myLayerstack.PushLayer(aLayer);
        aLayer->OnAttach();
    }

    void Application::PushOverlay(Layer* aOverlay)
    {
        myLayerstack.PushOverlay(aOverlay);
        aOverlay->OnAttach();
    }

    void Application::OnEvent(Event& aEvent)
    {
        EventDispatcher dispatcher(aEvent);
        dispatcher.Dispatch<WindowCloseEvent>(GF_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(GF_BIND_EVENT_FN(Application::OnWindowResize));

        for (auto it = myLayerstack.rbegin(); it != myLayerstack.rend(); ++it)
        {
            if (aEvent.myHandled)
            {
                break;
            }
            (*it)->OnEvent(aEvent);
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& aEvent)
    {
        myRunningState = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& aEvent)
    {
        if (aEvent.GetWidth() == 0 || aEvent.GetHeight() == 0)
        {
            myMinimized = true;
            return false;
        }

        myMinimized = false;
        Renderer::OnWindowResize(aEvent.GetWidth(), aEvent.GetHeight());

        return false;
    }
}