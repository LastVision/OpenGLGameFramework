#include "precompile.h"
#include "SubTexture2D.h"

namespace Framework
{
    SubTexture2D::SubTexture2D(const Ref<Texture2D>& aTexture, const glm::vec2& aMin, const glm::vec2& aMax)
        : myTexture(aTexture)
    {
        myTextureCoordinates[0] = { aMin.x, aMin.y };
        myTextureCoordinates[1] = { aMax.x, aMin.y };
        myTextureCoordinates[2] = { aMax.x, aMax.y };
        myTextureCoordinates[3] = { aMin.x, aMax.y };
    }

    Ref<SubTexture2D> SubTexture2D::CreateFromCoordinates(const Ref<Texture2D>& aTexture, const glm::vec2& aCoordinates, const glm::vec2& aCellSize, const glm::vec2& aSpriteSize)
    {
        glm::vec2 min = { (aCoordinates.x * aCellSize.x) / aTexture->GetWidth(), (aCoordinates.y * aCellSize.y) / aTexture->GetHeight() };
        glm::vec2 max = { ((aCoordinates.x + aSpriteSize.x) * aCellSize.x) / aTexture->GetWidth(), ((aCoordinates.y + aSpriteSize.y) * aCellSize.y) / aTexture->GetHeight() };

        return CreateRef<SubTexture2D>(aTexture, min, max);
    }
}