#include "precompile.h"
#include "LayerStack.h"

namespace Framework
{
    LayerStack::~LayerStack()
    {
        for (auto* layer : myLayers)
        {
            layer->OnDeattach();
            delete layer;
        }
        myLayers.clear();
    }

    void LayerStack::PushLayer(Layer* aLayer)
    {
        myLayers.emplace(myLayers.begin() + myLayerInsertIndex, aLayer);
        myLayerInsertIndex++;
    }

    void LayerStack::PushOverlay(Layer* aOverlay)
    {
        myLayers.emplace_back(aOverlay);
    }

    void LayerStack::PopLayer(Layer* aLayer)
    {
        auto it = std::find(myLayers.begin(), myLayers.begin() + myLayerInsertIndex, aLayer);
        if (it != myLayers.begin() + myLayerInsertIndex)
        {
            myLayers.erase(it);
            myLayerInsertIndex--;
        }
    }

    void LayerStack::PopOverlay(Layer* aOverlay)
    {
        auto it = std::find(myLayers.begin() + myLayerInsertIndex, myLayers.end(), aOverlay);
        if (it != myLayers.end())
        {
            myLayers.erase(it);
        }
    }
}