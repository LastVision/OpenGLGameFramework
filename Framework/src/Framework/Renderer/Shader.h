#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Framework
{
    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual const std::string& GetName() const = 0;

        virtual void SetMat3(const std::string& aName, const glm::mat3& aMatrix) = 0;
        virtual void SetMat4(const std::string& aName, const glm::mat4& aMatrix) = 0;

        virtual void SetFloat(const std::string& aName, const float aValue) = 0;
        virtual void SetFloat2(const std::string& aName, const glm::vec2& aValue) = 0;
        virtual void SetFloat3(const std::string& aName, const glm::vec3& aValue) = 0;
        virtual void SetFloat4(const std::string& aName, const glm::vec4& aValue) = 0;
         
        virtual void SetInt(const std::string& aName, const int aValue) = 0;
        virtual void SetIntArray(const std::string& aName, int* someValues, uint32_t aCount) = 0;

        static Ref<Shader> Create(const std::string& aName, const std::string& aVertexSrc, const std::string& aFragmentSrc);
        static Ref<Shader> Create(const std::string& aFilePath);
    };

    class ShaderLibrary
    {
    public:
        void Add(const std::string& aName, const Ref<Shader>& aShader);
        void Add(const Ref<Shader>& aShader);
        Ref<Shader> Load(const std::string& aFilepath);
        Ref<Shader> Load(const std::string& aName, const std::string& aFilepath);

        Ref<Shader> Get(const std::string& aName);

        bool Exists(const std::string& aName);
    private:
        std::unordered_map<std::string, Ref<Shader>> myShaders;
    };
}