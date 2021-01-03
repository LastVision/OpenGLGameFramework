#include "precompile.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Framework
{
    SceneCamera::SceneCamera()
    {
        RecalculateProjection();
    }

    void SceneCamera::SetPerspective(float aVerticalFOV, float aNearClip, float aFarClip)
    {
        myProjectionType = ProjectionType::Perspective;
        myPerspectiveFOV = aVerticalFOV;
        myPerspectiveNear = aNearClip;
        myPerspectiveFar = aFarClip;

        RecalculateProjection();
    }

    void SceneCamera::SetOrthographic(float aSize, float aNearClip, float aFarClip)
    {
        myProjectionType = ProjectionType::Orthographic;
        myOrthographicSize = aSize;
        myOrthographicNear = aNearClip;
        myOrthographicFar = aFarClip;

        RecalculateProjection();
    }

    void SceneCamera::SetViewportSize(uint32_t aWidth, uint32_t aHeight)
    {
        myAspectRatio = (float)aWidth / (float)aHeight;

        RecalculateProjection();
    }

    void SceneCamera::RecalculateProjection()
    {
        if (myProjectionType == ProjectionType::Perspective)
        {
            myProjection = glm::perspective(myPerspectiveFOV, myAspectRatio, myPerspectiveNear, myPerspectiveFar);
        }
        else
        {
            const float orthoLeft = -myOrthographicSize * myAspectRatio * 0.5f;
            const float orthoRight = myOrthographicSize * myAspectRatio * 0.5f;
            const float orthoBottom = -myOrthographicSize * 0.5f;
            const float orthoTop = myOrthographicSize * 0.5f;

            myProjection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, myOrthographicNear, myOrthographicFar);
        }
    }
}