#include "precompile.h"
#include "EditorCamera.h"
#include "Framework/Core/Input.h"
#include "Framework/Core/KeyCodes.h"
#include "Framework/Core/MouseCodes.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


namespace Framework
{
    EditorCamera::EditorCamera(float aFov, float aAspectRatio, float aNearClip, float aFarClip)
        : myFOV(aFov), myAspectRatio(aAspectRatio), myNearClip(aNearClip), myFarClip(aFarClip), Camera(glm::perspective(glm::radians(aFov), aAspectRatio, aNearClip, aFarClip))
    {
        UpdateView();
    }

    void EditorCamera::OnUpdate(Timestep& aTimestep)
    {
        if (Input::IsKeyPressed(Key::LeftAlt))
        {
            const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
            glm::vec2 delta = (mouse - myInitalMousePosition) * 0.003f;
            myInitalMousePosition = mouse;

            if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
                MousePan(delta);
            if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
                MouseRotate(delta);
            if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
                MouseZoom(delta.y);
        }

        UpdateView();
    }

    void EditorCamera::OnEvent(Event& aEvent)
    {
        EventDispatcher dispatcher(aEvent);
        dispatcher.Dispatch<MouseScrolledEvent>(GF_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
    }

    glm::vec3 EditorCamera::GetUpDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetRightDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetForwardDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::quat EditorCamera::GetOrientation() const
    {
        return glm::quat(glm::vec3(-myPitch, -myYaw, 0.0f));
    }

    void EditorCamera::UpdateProjection()
    {
        myAspectRatio = myViewportWidth / myViewportHeight;
        myProjection = glm::perspective(glm::radians(myFOV), myAspectRatio, myNearClip, myFarClip);
    }

    void EditorCamera::UpdateView()
    {
        myPosition = CalculatePosition();

        glm::quat orientation = GetOrientation();
        myViewMatrix = glm::translate(glm::mat4(1.0f), myPosition) * glm::toMat4(orientation);
        myViewMatrix = glm::inverse(myViewMatrix);
    }

    bool EditorCamera::OnMouseScroll(MouseScrolledEvent& aEvent)
    {
        float delta = aEvent.GetYOffset() * 0.1f;
        MouseZoom(delta);
        UpdateView();
        return false;
    }

    void EditorCamera::MousePan(const glm::vec2& aDelta)
    {
        auto [xSpeed, ySpeed] = PanSpeed();
        myFocalPoint += -GetRightDirection() * aDelta.x * xSpeed * myDistance;
        myFocalPoint += GetUpDirection() * aDelta.y * ySpeed * myDistance;
    }

    void EditorCamera::MouseRotate(const glm::vec2& aDelta)
    {
        float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
        myYaw += yawSign * aDelta.x * RotationSpeed();
        myPitch += aDelta.y * RotationSpeed();
    }

    void EditorCamera::MouseZoom(const float aDelta)
    {
        myDistance -= aDelta * ZoomSpeed();
        if (myDistance < 1.0f)
        {
            myFocalPoint += GetForwardDirection();
            myDistance = 1.0f;
        }
    }

    glm::vec3 EditorCamera::CalculatePosition()
    {
        return myFocalPoint - GetForwardDirection() * myDistance;
    }

    std::pair<float, float> EditorCamera::PanSpeed() const
    {
        float x = std::min(myViewportWidth / 1000.0f, 2.4f); // max = 2.4f
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(myViewportHeight / 1000.0f, 2.4f); // max = 2.4f
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return { xFactor, yFactor };
    }

    float EditorCamera::RotationSpeed() const
    {
        return 0.8f;
    }

    float EditorCamera::ZoomSpeed() const
    {
        float distance = myDistance * 0.2f;
        distance = std::max(distance, 0.0f);
        float speed = distance * distance;
        speed = std::min(speed, 100.0f); // max speed = 100
        return speed;
    }
}