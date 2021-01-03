#include "precompile.h"
#include "OrthographicCameraController.h"

#include "Framework/Core/Input.h"
#include "Framework/Core/KeyCodes.h"

namespace Framework
{
    OrthographicCameraController::OrthographicCameraController(float aAspectRatio, bool aRotationEnabledState)
        : myAspectRatio(aAspectRatio),
        myBounds({ -myAspectRatio * myZoomLevel, myAspectRatio * myZoomLevel, -myZoomLevel, myZoomLevel }),
        myCamera(myBounds.myLeft, myBounds.myRight, myBounds.myBottom, myBounds.myTop), 
        myRotationEnable(aRotationEnabledState)
    {
    }

    void OrthographicCameraController::OnUpdate(Timestep aTimestep)
    {
        GF_PROFILE_FUNCTION();

        if (Input::IsKeyPressed(KeyCode::W))
        {
            myCameraPosition.y -= myCameraTranslationSpeed * aTimestep;
        }
        else if (Input::IsKeyPressed(KeyCode::S))
        {
            myCameraPosition.y += myCameraTranslationSpeed * aTimestep;
        }

        if (Input::IsKeyPressed(KeyCode::A))
        {
            myCameraPosition.x -= myCameraTranslationSpeed * aTimestep;
        }
        else if (Input::IsKeyPressed(KeyCode::D))
        {
            myCameraPosition.x += myCameraTranslationSpeed * aTimestep;
        }

        if (myRotationEnable)
        {
            if (Input::IsKeyPressed(KeyCode::Q))
            {
                myCameraRotation += myCameraRotationSpeed * aTimestep;
            }
            else if (Input::IsKeyPressed(KeyCode::E))
            {
                myCameraRotation -= myCameraRotationSpeed * aTimestep;
            }
            myCamera.SetRotation(myCameraRotation);
        }

        myCamera.SetPosition(myCameraPosition);

        myCameraTranslationSpeed = myZoomLevel;
    }

    void OrthographicCameraController::OnEvent(Event& aEvent)
    {
        GF_PROFILE_FUNCTION();

        EventDispatcher dispatcher(aEvent);
        dispatcher.Dispatch<MouseScrolledEvent>(GF_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(GF_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    void OrthographicCameraController::OnResize(const float aWidth, const float aHeight)
    {
        GF_PROFILE_FUNCTION();

        myAspectRatio = aWidth / aHeight;
        CalculateView();
    }

    void OrthographicCameraController::CalculateView()
    {
        myBounds = { -myAspectRatio * myZoomLevel, myAspectRatio * myZoomLevel, -myZoomLevel, myZoomLevel };
        myCamera.SetProjection(myBounds.myLeft, myBounds.myRight, myBounds.myBottom, myBounds.myTop);
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& aEvent)
    {
        GF_PROFILE_FUNCTION();

        myZoomLevel -= aEvent.GetYOffset() * 0.25f;
        myZoomLevel = std::max(myZoomLevel, 0.25f);

        CalculateView();

        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& aEvent)
    {
        GF_PROFILE_FUNCTION();

        OnResize(static_cast<float>(aEvent.GetWidth()), static_cast<float>(aEvent.GetHeight()));
        return false;
    }
}