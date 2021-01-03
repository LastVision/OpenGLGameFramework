#pragma once

#include "entt.hpp"
#include "Framework/Core/Timestep.h"
#include "Framework/Renderer/EditorCamera.h"

namespace Framework
{
    class Entity;

    class Scene
    {
    private:
        friend class Entity;
        friend class SceneHierarchyPanel;
        friend class SceneSerializer;
    public:
        Scene();
        ~Scene();
        
        Entity CreateEntity(const std::string& aName = std::string());
        void DestroyEntity(Entity anEntity);

        void OnUpdateRuntime(Timestep aTimestep);
        void OnUpdateEditor(Timestep aTimestep, EditorCamera& camera);
        void OnViewportResize(const uint32_t aWidth, const uint32_t aHeight);

        Entity GetPrimaryCameraEntity();
    private:
        template<typename T>
        void OnComponentAdded(Entity anEntity, T& aComponent);
    private:
        entt::registry myRegistry;

        uint32_t myViewportWidth = 0;
        uint32_t myViewportHeight = 0;
    };
}