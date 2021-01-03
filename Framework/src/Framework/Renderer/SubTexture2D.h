#pragma once

#include <glm/glm.hpp>
#include "Texture.h"

namespace Framework
{
    class SubTexture2D
    {
    public:
        SubTexture2D(const Ref<Texture2D>& aTexture, const glm::vec2& aMin, const glm::vec2& aMax);
        virtual ~SubTexture2D() = default;

        const Ref<Texture2D> GetTexture() const { return myTexture; }
        const glm::vec2* GetTextureCoordinates() const { return myTextureCoordinates; }

        static Ref<SubTexture2D> CreateFromCoordinates(const Ref<Texture2D>& aTexture, const glm::vec2& aCoordinates, const glm::vec2& aCellSize, const glm::vec2& aSpriteSize = { 1, 1 });
    private:
        Ref<Texture2D> myTexture;
        glm::vec2 myTextureCoordinates[4];
    };
}