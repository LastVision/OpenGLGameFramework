#pragma once
#include "Framework.h"

class GameLayer : public Framework::Layer
{
public:
    GameLayer();
    virtual ~GameLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDeattach() override;

    virtual void OnUpdate(Framework::Timestep aTimestep) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Framework::Event & aEvent) override;
private:
    Framework::OrthographicCameraController myCameraController;
};
