#include "precompile.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Framework
{
    void OpenGLRendererAPI::Init()
    {
        GF_PROFILE_FUNCTION();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::SetViewport(uint32_t aX, uint32_t aY, uint32_t aWidth, uint32_t aHeight)
    {
        glViewport(aX, aY, aWidth, aHeight);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& aColor)
    {
        glClearColor(aColor.r, aColor.g, aColor.b, aColor.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& aVertexArray, uint32_t aIndexCount)
    {
        uint32_t count = aIndexCount ? aVertexArray->GetIndexBuffer()->GetCount() : aIndexCount;

        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}