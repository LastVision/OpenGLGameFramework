#pragma once
#include "Framework/Renderer/RendererAPI.h"

namespace Framework
{
    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        virtual void Init() override;

        virtual void SetViewport(uint32_t aX, uint32_t aY, uint32_t aWidth, uint32_t aHeight) override;

        virtual void SetClearColor(const glm::vec4& aColor) override;
        virtual void Clear() override;

        virtual void DrawIndexed(const Ref<VertexArray>& aVertexArray, uint32_t aIndexCount = 0) override;
    private:
    };
}