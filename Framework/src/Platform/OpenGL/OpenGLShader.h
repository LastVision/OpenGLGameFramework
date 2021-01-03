#pragma once
#include "Framework/Renderer/Shader.h"

typedef unsigned int GLenum;

namespace Framework
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& aName, const std::string& aVertexSrc, const std::string& aFragmentSrc);
        OpenGLShader(const std::string& aFilePath);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual const std::string& GetName() const override { return myName; }

        virtual void SetMat3(const std::string& aName, const glm::mat3& aMatrix) override { UploadUniformMat3(aName, aMatrix); }
        virtual void SetMat4(const std::string& aName, const glm::mat4& aMatrix) override { UploadUniformMat4(aName, aMatrix); }

        virtual void SetFloat(const std::string& aName, const float aValue) override { UploadUniformFloat(aName, aValue); }
        virtual void SetFloat2(const std::string& aName, const glm::vec2& aValue) override { UploadUniformFloat2(aName, aValue); }
        virtual void SetFloat3(const std::string& aName, const glm::vec3& aValue) override { UploadUniformFloat3(aName, aValue); }
        virtual void SetFloat4(const std::string& aName, const glm::vec4& aValue) override { UploadUniformFloat4(aName, aValue); }

        virtual void SetInt(const std::string& aName, const int aValue) override { UploadUniformInt(aName, aValue); }
        virtual void SetIntArray(const std::string& aName, int* someValues, uint32_t aCount) override { UploadUniformIntArray(aName, someValues, aCount); }

        void UploadUniformMat3(const std::string& aName, const glm::mat4& aMatrix);
        void UploadUniformMat4(const std::string& aName, const glm::mat4& aMatrix);

        void UploadUniformFloat(const std::string& aName, const float aValue);
        void UploadUniformFloat2(const std::string& aName, const glm::vec2& aValue);
        void UploadUniformFloat3(const std::string& aName, const glm::vec3& aValue);
        void UploadUniformFloat4(const std::string& aName, const glm::vec4& aValue);

        void UploadUniformInt(const std::string& aName, const int aValue);
        void UploadUniformIntArray(const std::string& aName, int* someValues, uint32_t aCount);
    private:
        std::string ReadFile(const std::string& aFilePath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& aSource);
        void Compile(const std::unordered_map<GLenum, std::string>& aShaderSources);
    private:
        uint32_t myRendererID = 0;
        std::string myName;
    };
}