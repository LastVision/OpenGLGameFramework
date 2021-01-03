#pragma once
#include "RendererAPI.h"

namespace Framework
{
    class RenderCommand
    {
    public:
        inline static void Init()
        {
            ourRendererAPI->Init();
        }

        inline static void SetViewport(uint32_t aX, uint32_t aY, uint32_t aWidth, uint32_t aHeight)
        {
            ourRendererAPI->SetViewport(aX, aY, aWidth, aHeight);
        }

        inline static void SetClearColor(const glm::vec4& aColor)
        {
            ourRendererAPI->SetClearColor(aColor);
        }

        inline static void Clear()
        {
            ourRendererAPI->Clear();
        }

        inline static void DrawIndexed(const Ref<VertexArray>& aVertexArray, uint32_t aIndexCount = 0)
        {
            ourRendererAPI->DrawIndexed(aVertexArray, aIndexCount);
        }
    private:
        static Scope<RendererAPI> ourRendererAPI;
    };
}