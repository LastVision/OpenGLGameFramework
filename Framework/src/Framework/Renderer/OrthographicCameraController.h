#pragma once
#include "Framework/Renderer/OrthographicCamera.h"
#include "Framework/Core/Timestep.h"

#include "Framework/Event/MouseEvent.h"
#include "Framework/Event/ApplicationEvent.h"

namespace Framework
{
    struct OrthographicCameraBounds
    {
        float myLeft = 0, myRight = 0;
        float myBottom = 0, myTop = 0;

        float GetWidth() const { return myRight - myLeft; }
        float GetHeight() const { return myTop - myBottom; }
    };

    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aAspectRatio, bool aRotationEnabledState = false);

        void OnUpdate(Timestep aTimestep);
        void OnEvent(Event& aEvent);

        void OnResize(const float aWidth, const float aHeight);

        OrthographicCamera& GetCamera() { return myCamera; }
        const OrthographicCamera& GetCamera() const { return myCamera; }

        void SetZoomLevel(const float aLevel) { myZoomLevel = aLevel; CalculateView(); }
        const float GetZoomLevel() const { return myZoomLevel; }

        const OrthographicCameraBounds& GetBounds() const { return myBounds; }
    private:
        void CalculateView();

        bool OnMouseScrolled(MouseScrolledEvent& aEvent);
        bool OnWindowResized(WindowResizeEvent& aEvent);
    private:
        float myAspectRatio;
        float myZoomLevel = 1.0f;
        OrthographicCamera myCamera;
        OrthographicCameraBounds myBounds;

        bool myRotationEnable;

        glm::vec3 myCameraPosition = { 0.0f, 0.0f, 0.0f };
        float myCameraRotation = 0.0f;
        float myCameraTranslationSpeed = 2.0f, myCameraRotationSpeed = 180.0f;

    };
}