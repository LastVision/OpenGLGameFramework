#include "precompile.h"
#include "Renderer.h"
#include "Renderer2D.h"

namespace Framework
{
    Scope<Renderer::SceneData> Renderer::ourSceneData = CreateScope<Renderer::SceneData>();

    void Renderer::Init()
    {
        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::OnWindowResize(uint32_t aWidth, uint32_t aHeight)
    {
        RenderCommand::SetViewport(0, 0, aWidth, aHeight);
    }

    void Renderer::BeginScene(const OrthographicCamera& aCamera)
    {
        ourSceneData->myViewProjectionMatrix = aCamera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const Ref<Shader>& aShader, const Ref<VertexArray>& aVertexArray, const glm::mat4& aTransform)
    {
        aShader->Bind();
        aShader->SetMat4("uViewProjection", ourSceneData->myViewProjectionMatrix);
        aShader->SetMat4("uTransform", aTransform);

        aVertexArray->Bind();
        RenderCommand::DrawIndexed(aVertexArray);
    }
}