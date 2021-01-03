#pragma once
#include "Entity.h"

namespace Framework
{
    class ScriptableEntity
    {
    public:
        virtual ~ScriptableEntity() {}

        template<typename T>
        T& GetComponent()
        {
            return myEntity.GetComponent<T>();
        }

    protected:
        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void OnUpdate(Timestep aTimestep) {}
    private:
        Entity myEntity;
        friend class Scene;
    };
}