#include "precompile.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Framework
{
    static GLenum ShaderTypeFromString(const std::string& aType)
    {
        if (aType == "vertex")
            return GL_VERTEX_SHADER;
        if (aType == "fragment" || aType == "pixel")
            return GL_FRAGMENT_SHADER;

        GF_CORE_ASSERT(false, "Unkown shader type!");
        return 0;
    }

    OpenGLShader::OpenGLShader(const std::string& aName, const std::string& aVertexSrc, const std::string& aFragmentSrc)
        : myName(aName)
    {
        GF_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSources;
        shaderSources[GL_VERTEX_SHADER] = aVertexSrc;
        shaderSources[GL_FRAGMENT_SHADER] = aFragmentSrc;
        Compile(shaderSources);
    }

    OpenGLShader::OpenGLShader(const std::string& aFilePath)
    {
        GF_PROFILE_FUNCTION();

        std::string source = ReadFile(aFilePath);
        auto shaderSources = PreProcess(source);
        Compile(shaderSources);

        //Extract name from filepath
        auto lastSlashPos = aFilePath.find_last_of("/\\");
        lastSlashPos = lastSlashPos == std::string::npos ? 0 : lastSlashPos + 1;
        auto lastDot = aFilePath.rfind('.');
        auto count = lastDot == std::string::npos ? aFilePath.size() - lastSlashPos : lastDot - lastSlashPos;
        myName = aFilePath.substr(lastSlashPos, count);
    }

    OpenGLShader::~OpenGLShader()
    {
        GF_PROFILE_FUNCTION();

        glDeleteProgram(myRendererID);
    }

    std::string OpenGLShader::ReadFile(const std::string& aFilePath)
    {
        GF_PROFILE_FUNCTION();

        std::string result;
        std::ifstream in(aFilePath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            GF_CORE_LOG_ERROR("Could not open file '{0}'", aFilePath);
        }
        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& aSource)
    {
        GF_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        const size_t typeTokenLength = std::strlen(typeToken);
        size_t pos = aSource.find(typeToken, 0);
        while (pos != std::string::npos)
        {
            const size_t eol = aSource.find_first_of("\r\n", pos);
            GF_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            const size_t begin = pos + typeTokenLength + 1;
            const std::string type = aSource.substr(begin, eol - begin);
            GF_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified.");

            const size_t nextLinePos = aSource.find_first_not_of("\r\n", eol);
            GF_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");

            pos = aSource.find(typeToken, nextLinePos);
            shaderSources[ShaderTypeFromString(type)] = pos == std::string::npos? aSource.substr(nextLinePos) : aSource.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& aShaderSources)
    {
        GF_PROFILE_FUNCTION();

        GLuint program = glCreateProgram();
        GF_CORE_ASSERT(aShaderSources.size() <= 2, "We only support 2 shaders for now");
        std::array<GLenum, 2> glShaderIDs;
        int glShaderIDIndex = 0;
        for (auto& kv : aShaderSources)
        {
            GLenum type = kv.first;
            const std::string& source = kv.second;

            GLuint shader = glCreateShader(type);
            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                GF_CORE_LOG_ERROR("{0}", infoLog.data());
                GF_CORE_ASSERT(false, "Shader compilation failure!");
                break;
            }
            glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)& isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog;
            infoLog.reserve(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);

            for (auto id : glShaderIDs)
                glDeleteShader(id);

            GF_CORE_LOG_ERROR("{0}", infoLog.data());
            GF_CORE_ASSERT(false, "Shader link failure!");
            return;
        }

        for (auto id : glShaderIDs)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }

        myRendererID = program;
    }

    void OpenGLShader::Bind() const
    {
        GF_PROFILE_FUNCTION();

        glUseProgram(myRendererID);
    }

    void OpenGLShader::Unbind() const
    {
        GF_PROFILE_FUNCTION();

        glUseProgram(0);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& aName, const glm::mat4& aMatrix)
    {
        GF_PROFILE_FUNCTION();

        GLint location = glGetUniformLocation(myRendererID, aName.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(aMatrix));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& aName, const glm::mat4& aMatrix)
    {
        GF_PROFILE_FUNCTION();

        GLint location = glGetUniformLocation(myRendererID, aName.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(aMatrix));
    }

    void OpenGLShader::UploadUniformFloat(const std::string& aName, const float aValue)
    {
        GF_PROFILE_FUNCTION();

        GLint location = glGetUniformLocation(myRendererID, aName.c_str());
        glUniform1f(location, aValue);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& aName, const glm::vec2& aValue)
    {
        GF_PROFILE_FUNCTION();

        GLint location = glGetUniformLocation(myRendererID, aName.c_str());
        glUniform2f(location, aValue.x, aValue.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& aName, const glm::vec3& aValue)
    {
        GF_PROFILE_FUNCTION();

        GLint location = glGetUniformLocation(myRendererID, aName.c_str());
        glUniform3f(location, aValue.x, aValue.y, aValue.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& aName, const glm::vec4& aValue)
    {
        GF_PROFILE_FUNCTION();

        GLint location = glGetUniformLocation(myRendererID, aName.c_str());
        glUniform4f(location, aValue.x, aValue.y, aValue.z, aValue.w);
    }

    void OpenGLShader::UploadUniformInt(const std::string& aName, const int aValue)
    {
        GF_PROFILE_FUNCTION();

        GLint location = glGetUniformLocation(myRendererID, aName.c_str());
        glUniform1i(location, aValue);
    }

    void OpenGLShader::UploadUniformIntArray(const std::string& aName, int* someValues, uint32_t aCount)
    {
        GF_PROFILE_FUNCTION();

        GLint location = glGetUniformLocation(myRendererID, aName.c_str());
        glUniform1iv(location, aCount, someValues);
    }

}