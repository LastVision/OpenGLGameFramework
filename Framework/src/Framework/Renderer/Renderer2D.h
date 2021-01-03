#pragma once
#include "OrthographicCamera.h"
#include "Camera.h"
#include "EditorCamera.h"

#include "Texture.h"
#include "SubTexture2D.h"

namespace Framework
{
    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const Camera& aCamera, const glm::mat4& aTransform);
        static void BeginScene(const EditorCamera& aCamera);
        static void BeginScene(const OrthographicCamera& aCamera); // TODO: Remove
        static void EndScene();
        static void Flush();

        //Helper functions
        static void FillQuadVertexBuffer(const glm::vec3& aPosition, const glm::vec2& aSize, const float aRotation, const glm::vec4& aColor, const float aTextureId, const float aTilingFactor, const glm::vec2 aTextureCoords[4]);
        static void FillQuadVertexBuffer(const glm::mat4& aTransform, const glm::vec4& aColor, const float aTextureId, const float aTilingFactor, const glm::vec2 aTextureCoords[4]);
        static float GetTextureIndex(const Ref<Texture2D>& aTexture);

        //Primitives
        static void DrawQuad(const glm::vec2& aPosition, const glm::vec2& aSize, const glm::vec4& aColor);
        static void DrawQuad(const glm::vec3& aPosition, const glm::vec2& aSize, const glm::vec4& aColor);
        static void DrawQuad(const glm::vec2& aPosition, const glm::vec2& aSize, const Ref<Texture2D>& aTexture, const float aTilingFactor = 1.0f, const glm::vec4& aColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& aPosition, const glm::vec2& aSize, const Ref<Texture2D>& aTexture, const float aTilingFactor = 1.0f, const glm::vec4& aColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec2& aPosition, const glm::vec2& aSize, const Ref<SubTexture2D>& aSubTexture, const float aTilingFactor = 1.0f, const glm::vec4& aColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& aPosition, const glm::vec2& aSize, const Ref<SubTexture2D>& aSubTexture, const float aTilingFactor = 1.0f, const glm::vec4& aColor = glm::vec4(1.0f));

        static void DrawQuad(const glm::vec2& aPosition, const glm::vec2& aSize, const float aRotation, const glm::vec4& aColor);
        static void DrawQuad(const glm::vec3& aPosition, const glm::vec2& aSize, const float aRotation, const glm::vec4& aColor);
        static void DrawQuad(const glm::vec2& aPosition, const glm::vec2& aSize, const float aRotation, const Ref<Texture2D>& aTexture, const float aTilingFactor = 1.0f, const glm::vec4& aColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& aPosition, const glm::vec2& aSize, const float aRotation, const Ref<Texture2D>& aTexture, const float aTilingFactor = 1.0f, const glm::vec4& aColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec2& aPosition, const glm::vec2& aSize, const float aRotation, const Ref<SubTexture2D>& aSubTexture, const float aTilingFactor = 1.0f, const glm::vec4& aColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& aPosition, const glm::vec2& aSize, const float aRotation, const Ref<SubTexture2D>& aSubTexture, const float aTilingFactor = 1.0f, const glm::vec4& aColor = glm::vec4(1.0f));

        static void DrawQuad(const glm::mat4& aTransform, const glm::vec4& aColor);
        static void DrawQuad(const glm::mat4& aTransform, const Ref<Texture2D>& aTexture, const float aTilingFactor = 1.0f, const glm::vec4& aColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::mat4& aTransform, const Ref<SubTexture2D>& aSubTexture, const float aTilingFactor = 1.0f, const glm::vec4& aColor = glm::vec4(1.0f));
    
        //Stats
        struct Statistics
        {
            uint32_t myDrawCalls = 0;
            uint32_t myQuadCounts = 0;

            uint32_t GetTotalVertexCount() { return myQuadCounts * 4; }
            uint32_t GetTotalIndexCount() { return myQuadCounts * 6; }
        };
        static void ResetStats();
        static Statistics GetStats();
    private:
        static void FlushAndReset();
    };
}