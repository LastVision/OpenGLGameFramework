#pragma once

#include "Framework/Core/Layer.h"

namespace Framework
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDeattach() override;
        virtual void OnEvent(Event& aEvent) override;

        void Begin();
        void End();

        void BlockEvents(bool aBlockState) { myBlockEvents = aBlockState; }

        void SetDarkThemeColors();
    private:
        bool myBlockEvents = true;
        double myTime = 0;
    };
}