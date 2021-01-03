#include "precompile.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Framework
{
    OrthographicCamera::OrthographicCamera(const float aLeft, const float aRight, const float aBottom, const float aTop)
        : myProjectionMatrix(glm::ortho(aLeft, aRight, aBottom, aTop, -1.0f, 1.0f)), myViewMatrix(1.0f), myPosition(0.0), myRotation(0.0f)
    {
        GF_PROFILE_FUNCTION();

        myViewProjectionMatrix = myProjectionMatrix * myViewMatrix;
    }

    void OrthographicCamera::SetProjection(const float aLeft, const float aRight, const float aBottom, const float aTop)
    {
        GF_PROFILE_FUNCTION();

        myProjectionMatrix = glm::ortho(aLeft, aRight, aBottom, aTop, -1.0f, 1.0f);
        myViewProjectionMatrix = myProjectionMatrix * myViewMatrix;
    }

    void OrthographicCamera::RecaluclateViewMatrix()
    {
        GF_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), myPosition) * glm::rotate(glm::mat4(1.0f), glm::radians(myRotation), glm::vec3(0, 0, 1));

        myViewMatrix = glm::inverse(transform);
        myViewProjectionMatrix = myProjectionMatrix * myViewMatrix;
    }
}