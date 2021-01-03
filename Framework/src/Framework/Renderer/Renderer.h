#pragma once
#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

namespace Framework
{
    class Renderer
    {
    public:
        static void Init();
        static void OnWindowResize(uint32_t aWidth, uint32_t aHeight);

        static void BeginScene(const OrthographicCamera& aCamera);
        static void EndScene();

        static void Submit(const Ref<Shader>& aShader, const Ref<VertexArray>& aVertexArray, const glm::mat4& aTransform = glm::mat4(1.0f));

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    private:
        struct SceneData
        {
            glm::mat4 myViewProjectionMatrix;
        };

        static Scope<SceneData> ourSceneData;
    };
}