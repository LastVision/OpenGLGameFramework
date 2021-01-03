#pragma once

#include "Framework/Core/Core.h"
#include "Framework/Core/Layer.h"

namespace Framework
{
    class LayerStack
    {
        using Layers = std::vector<Layer*>;
    public:
        LayerStack() : myLayerInsertIndex(0) {}
        ~LayerStack();

        void PushLayer(Layer* aLayer);
        void PushOverlay(Layer* aOverlay);
        void PopLayer(Layer* aLayer);
        void PopOverlay(Layer* aOverlay);

        Layers::iterator begin() { return myLayers.begin(); }
        Layers::reverse_iterator rbegin() { return myLayers.rbegin(); }
        Layers::iterator end() { return myLayers.end(); }
        Layers::reverse_iterator rend() { return myLayers.rend(); }
    private:
        Layers myLayers;
        uint32_t myLayerInsertIndex;
    };
}