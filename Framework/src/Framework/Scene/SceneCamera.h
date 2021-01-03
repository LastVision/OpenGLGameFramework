#pragma once
#include "Framework/Renderer/Camera.h"

namespace Framework
{
    class SceneCamera : public Camera
    {
    public:
        enum class ProjectionType { Perspective = 0, Orthographic = 1 };
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetViewportSize(uint32_t aWidth, uint32_t aHeight);

        void SetPerspective(float aVerticalFOV, float aNearClip, float aFarClip);
        void SetOrthographic(float aSize, float aNearClip, float aFarClip);

        float GetPerspectiveVerticalFOV() const { return myPerspectiveFOV; }
        float GetPerspectiveNearClip() const { return myPerspectiveNear; }
        float GetPerspectiveFarClip() const { return myPerspectiveFar; }
        void SetPerspectiveVerticalFOV(float aFOV) { myPerspectiveFOV = aFOV; RecalculateProjection(); }
        void SetPerspectiveNearClip(const float aNearClip) { myPerspectiveNear = aNearClip; RecalculateProjection(); }
        void SetPerspectiveFarClip(const float aFarClip) { myPerspectiveFar = aFarClip; RecalculateProjection(); }

        float GetOrthographicSize() const { return myOrthographicSize; }
        float GetOrthographicNearClip() const { return myOrthographicNear; }
        float GetOrthographicFarClip() const { return myOrthographicFar; }
        void SetOrthographicSize(float aSize) { myOrthographicSize = aSize; RecalculateProjection(); }
        void SetOrthographicNearClip(const float aNearClip) { myOrthographicNear = aNearClip; RecalculateProjection(); }
        void SetOrthographicFarClip(const float aFarClip) { myOrthographicFar = aFarClip; RecalculateProjection(); }

        ProjectionType GetProjectionType() const { return myProjectionType; }
        void SetProjectionType(ProjectionType projectionType) { myProjectionType = projectionType; RecalculateProjection(); }
    private:
        void RecalculateProjection();
    private:
        ProjectionType myProjectionType = ProjectionType::Orthographic;

        float myOrthographicSize = 10.0f;
        float myOrthographicNear = -1.0f;
        float myOrthographicFar = 1.0f;

        float myPerspectiveFOV = glm::radians(45.00f);
        float myPerspectiveNear = 0.01f;
        float myPerspectiveFar = 10000.0f;

        float myAspectRatio = 0.0f;
    };
}