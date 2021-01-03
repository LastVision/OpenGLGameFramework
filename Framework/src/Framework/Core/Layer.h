#pragma once
#include "Framework/Core/Core.h"
#include "Framework/Core/Timestep.h"
#include "Framework/Event/Event.h"

namespace Framework
{
    class Layer
    {
    public:
        Layer(const std::string& name = "Layer") : myDebugName(name) {}
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDeattach() {}
        virtual void OnUpdate(Timestep timestep) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return myDebugName; }
    private:
        std::string myDebugName;
    };
}