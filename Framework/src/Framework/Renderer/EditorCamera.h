#pragma once

#include "Camera.h"
#include "Framework/Core/Timestep.h"
#include "Framework/Event/Event.h"
#include "Framework/Event/MouseEvent.h"

#include <glm/glm.hpp>

namespace Framework
{
    class EditorCamera : public Camera
    {
    public:
        EditorCamera() = default;
        EditorCamera(float aFov, float aAspectRatio, float aNearClip, float aFarClip);

        void OnUpdate(Timestep& aTimestep);
        void OnEvent(Event& aEvent);

        inline float GetDistance() const { return myDistance; }
        inline void SetDistance(float aDistance) { myDistance = aDistance; }

        inline void SetViewportSize(float aWidth, float aHeight) { myViewportWidth = aWidth; myViewportHeight = aHeight; UpdateProjection(); }

        const glm::mat4& GetViewMatrix() const { return myViewMatrix; }
        glm::mat4 GetViewProjection() const { return myProjection * myViewMatrix; }

        glm::vec3 GetUpDirection() const;
        glm::vec3 GetRightDirection() const;
        glm::vec3 GetForwardDirection() const;
        const glm::vec3& GetPosition() const { return myPosition; }
        glm::quat GetOrientation() const;

        float GetPitch() const { return myPitch; }
        float GetYaw() const { return myYaw; }

    private:
        void UpdateProjection();
        void UpdateView();

        bool OnMouseScroll(MouseScrolledEvent& aEvent);

        void MousePan(const glm::vec2& aDelta);
        void MouseRotate(const glm::vec2& aDelta);
        void MouseZoom(const float aDelta);

        glm::vec3 CalculatePosition();

        std::pair<float, float> PanSpeed() const;
        float RotationSpeed() const;
        float ZoomSpeed() const;

    private:
        float myFOV = 45.0f;
        float myAspectRatio = 1.778f;
        float myNearClip = 0.1f;
        float myFarClip = 1000.0f;

        float myViewportWidth = 1280.0f;
        float myViewportHeight = 720.0f;
        glm::vec3 myPosition = { 0.0f, 0.0f, 0.0f };
        glm::vec3 myFocalPoint = { 0.0f, 0.0f, 0.0f };

        glm::vec2 myInitalMousePosition;

        float myPitch = 0.0f;
        float myYaw = 0.0f;
        float myDistance = 10.0f;

        glm::mat4 myViewMatrix;
    };
}