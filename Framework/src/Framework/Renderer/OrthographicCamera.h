#pragma once
#include <glm/glm.hpp>

namespace Framework
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(const float aLeft, const float aRight, const float aBottom, const float aTop);

        const glm::vec3& GetPosition() const { return myPosition; }
        void SetPosition(const glm::vec3& aPosition) { myPosition = aPosition; RecaluclateViewMatrix(); }

        const float GetRotation() const { return myRotation; }
        void SetRotation(const float& aRotation) { myRotation = aRotation; RecaluclateViewMatrix(); }

        void SetProjection(const float aLeft, const float aRight, const float aBottom, const float aTop);

        const glm::mat4& GetProjectionMatrix() const { return myProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const { return myViewMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const { return myViewProjectionMatrix; }
    private:
        void RecaluclateViewMatrix();
    private:
        glm::mat4 myProjectionMatrix;
        glm::mat4 myViewMatrix;
        glm::mat4 myViewProjectionMatrix;

        glm::vec3 myPosition;
        float myRotation = 0.0f;
    };
}