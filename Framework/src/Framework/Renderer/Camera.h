#pragma once
#include <glm/glm.hpp>

namespace Framework
{
    class Camera
    {
    public:
        Camera() = default;
        Camera(const glm::mat4& aProjection)
            : myProjection(aProjection) {}
        virtual ~Camera() = default;

        const glm::mat4& GetProjection() const { return myProjection; }
    protected:
        glm::mat4 myProjection = glm::mat4(1.0f);
    };
}