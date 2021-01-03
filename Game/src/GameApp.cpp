#include "Framework.h"
#include "Framework/Core/EntryPoint.h"
#include "GameLayer.h"

class GameApp : public Framework::Application
{
public:
    GameApp()
    {
        PushLayer(new GameLayer());
    }
    ~GameApp() = default;

private:

};

Framework::Application* Framework::CreateApplication()
{
    return new GameApp();
}