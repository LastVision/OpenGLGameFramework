#include "precompile.h"
#include "Entity.h"

namespace Framework
{
    Entity::Entity(entt::entity aHandle, Scene* aScene)
        : myEntityHandle(aHandle), myScene(aScene)
    {
    }
}