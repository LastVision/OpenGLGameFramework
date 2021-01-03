#pragma once
#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Framework
{
    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0, 
            OpenGL = 1
        };
    public:
        virtual ~RendererAPI() = default;
        virtual void Init() = 0;

        virtual void SetViewport(uint32_t aX, uint32_t aY, uint32_t aWidth, uint32_t aHeight) = 0;

        virtual void SetClearColor(const glm::vec4& aColor) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const Ref<VertexArray>& aVertexArray, uint32_t aIndexCount = 0) = 0;

        inline static API GetAPI() { return ourAPI; }
    private:
        static API ourAPI;
    };
}