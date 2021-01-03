#include "precompile.h"
#include "Shader.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Framework
{
    Ref<Shader> Shader::Create(const std::string& aName, const std::string& aVertexSrc, const std::string& aFragmentSrc)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:     GF_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLShader>(aName, aVertexSrc, aFragmentSrc);
        }

        GF_CORE_ASSERT(false, "Unkown RenderAPI!");
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& aFilePath)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:     GF_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:   return CreateRef<OpenGLShader>(aFilePath);
        }

        GF_CORE_ASSERT(false, "Unkown RenderAPI!");
        return nullptr;
    }

    // Shader Library

    void ShaderLibrary::Add(const std::string& aName, const Ref<Shader>& aShader)
    {
        GF_CORE_ASSERT(Exists(aName) == false, "Shader already exists!");
        myShaders[aName] = aShader;
    }

    void ShaderLibrary::Add(const Ref<Shader>& aShader)
    {
        auto& name = aShader->GetName();
        Add(name, aShader);
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& aFilepath)
    {
        auto shader = Shader::Create(aFilepath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& aName, const std::string& aFilepath)
    {
        auto shader = Shader::Create(aFilepath);
        Add(aName, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string& aName)
    {
        GF_CORE_ASSERT(Exists(aName), "Shader not found!");
        return myShaders[aName];
    }

    bool ShaderLibrary::Exists(const std::string& aName)
    {
        return myShaders.find(aName) != myShaders.end();
    }
}