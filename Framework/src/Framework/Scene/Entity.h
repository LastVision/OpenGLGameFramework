#pragma once
#include "Scene.h"

#include "entt.hpp"

namespace Framework
{
    class Entity
    {
    private:
        friend class EntityPropertiesPanel;
    public:
        Entity() = default;
        Entity(entt::entity aHandle, Scene* aScene);
        Entity(const Entity& aOther) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... someArgs)
        {
            GF_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
            T& component = myScene->myRegistry.emplace<T>(myEntityHandle, std::forward<Args>(someArgs)...);
            myScene->OnComponentAdded<T>(*this, component);
            return component;
        }

        template<typename T>
        T& GetComponent()
        {
            GF_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

            return myScene->myRegistry.get<T>(myEntityHandle);
        }

        template<typename T>
        bool HasComponent()
        {
            return myScene->myRegistry.has<T>(myEntityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            GF_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

            myScene->myRegistry.remove<T>(myEntityHandle);
        }

        operator bool() const { return myEntityHandle != entt::null; }
        operator entt::entity() const { return myEntityHandle; }
        operator uint32_t() const { return (uint32_t)myEntityHandle; }

        bool operator==(const Entity& other)const { return myEntityHandle == other.myEntityHandle && myScene == other.myScene; }
        bool operator!=(const Entity& other)const { return !(*this == other); }
    private:
        entt::entity myEntityHandle{ entt::null };
        Scene* myScene = nullptr;
    };
}