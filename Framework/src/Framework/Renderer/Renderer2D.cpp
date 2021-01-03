#include "precompile.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Framework
{
    static const glm::vec2 myDefaultTextureCoords[4] = { {0.0f,0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
    struct QuadVertex
    {
        glm::vec3 myPosition;
        glm::vec4 myColor;
        glm::vec2 myTexCoord;
        float myTextureID;
        float myTilingFactor;
        //TODO: add support for maskID
    };

    struct Renderer2DData
    {
        static const uint32_t MaxQuads = 10000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32; //TODO: RenderCaps

        Ref<VertexArray> myQuadVertexArray;
        Ref<VertexBuffer> myQuadVertexBuffer;
        Ref<Shader> myTextureShader;
        Ref<Texture2D> myWhiteTexture;

        uint32_t myQuadIndexCount = 0;
        QuadVertex* myQuadVertexBase = nullptr;
        QuadVertex* myQuadVertexPtr = nullptr;

        std::array<Ref<Texture2D>, MaxTextureSlots> myTextureSlots;
        uint32_t myTextureSlotIndex = 1; // 0 = white texture

        glm::vec4 myQuadVertexPosition[4];

        Renderer2D::Statistics myStatistics;
    };

    static Renderer2DData s_Data;

    void Renderer2D::Init()
    {
        GF_PROFILE_FUNCTION();

        s_Data.myQuadVertexArray = VertexArray::Create();

        s_Data.myQuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
        s_Data.myQuadVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "aPosition" },
            { ShaderDataType::Float4, "aColor" },
            { ShaderDataType::Float2, "aTexCoord" },
            { ShaderDataType::Float, "aTexIndex" },
            { ShaderDataType::Float, "aTilingFactor" }
            });
        s_Data.myQuadVertexArray->AddVertexBuffer(s_Data.myQuadVertexBuffer);

        s_Data.myQuadVertexBase = new QuadVertex[s_Data.MaxVertices];

        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> indexBuffer;
        indexBuffer = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
        s_Data.myQuadVertexArray->SetIndexBuffer(indexBuffer);

        delete[] quadIndices;

        s_Data.myWhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.myWhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        s_Data.myTextureShader = Shader::Create("assets/shaders/Texture.glsl");

        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; ++i)
            samplers[i] = i;

        s_Data.myTextureShader->Bind();
        s_Data.myTextureShader->SetIntArray("uTexture", samplers, s_Data.MaxTextureSlots);

        s_Data.myTextureSlots[0] = s_Data.myWhiteTexture;

        s_Data.myQuadVertexPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.myQuadVertexPosition[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.myQuadVertexPosition[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
        s_Data.myQuadVertexPosition[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
    }

    void Renderer2D::Shutdown()
    {
        GF_PROFILE_FUNCTION();

        delete[] s_Data.myQuadVertexBase;
    }

    void Renderer2D::BeginScene(const Camera& aCamera, const glm::mat4& aTransform)
    {
        GF_PROFILE_FUNCTION();

        glm::mat4 viewProj = aCamera.GetProjection() * glm::inverse(aTransform);

        s_Data.myTextureShader->Bind();
        s_Data.myTextureShader->SetMat4("uViewProjection", viewProj);

        s_Data.myQuadIndexCount = 0;
        s_Data.myQuadVertexPtr = s_Data.myQuadVertexBase;

        s_Data.myTextureSlotIndex = 1;
    }

    void Renderer2D::BeginScene(const EditorCamera& aCamera)
    {
        GF_PROFILE_FUNCTION();

        s_Data.myTextureShader->Bind();
        s_Data.myTextureShader->SetMat4("uViewProjection", aCamera.GetViewProjection());

        s_Data.myQuadIndexCount = 0;
        s_Data.myQuadVertexPtr = s_Data.myQuadVertexBase;

        s_Data.myTextureSlotIndex = 1;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& aCamera)
    {
        GF_PROFILE_FUNCTION();

        s_Data.myTextureShader->Bind();
        s_Data.myTextureShader->SetMat4("uViewProjection", aCamera.GetViewProjectionMatrix());

        s_Data.myQuadIndexCount = 0;
        s_Data.myQuadVertexPtr = s_Data.myQuadVertexBase;

        s_Data.myTextureSlotIndex = 1;
    }

    void Renderer2D::EndScene()
    {
        GF_PROFILE_FUNCTION();

        uint32_t dataSize = (uint8_t*)s_Data.myQuadVertexPtr - (uint8_t*)s_Data.myQuadVertexBase;
        s_Data.myQuadVertexBuffer->SetData(s_Data.myQuadVertexBase, dataSize);

        Flush();
    }

    void Renderer2D::Flush()
    {
        GF_PROFILE_FUNCTION();

        if (s_Data.myQuadIndexCount == 0)
        {
            return;
        }

        for (uint32_t i = 0; i < s_Data.myTextureSlotIndex; ++i)
        {
            s_Data.myTextureSlots[i]->Bind(i);
        }

        RenderCommand::DrawIndexed(s_Data.myQuadVertexArray, s_Data.myQuadIndexCount);
        ++s_Data.myStatistics.myDrawCalls;
    }

    void Renderer2D::FlushAndReset()
    {
        GF_PROFILE_FUNCTION();

        EndScene();

        s_Data.myQuadIndexCount = 0;
        s_Data.myQuadVertexPtr = s_Data.myQuadVertexBase;

        s_Data.myTextureSlotIndex = 1;
    }

    void Renderer2D::FillQuadVertexBuffer(const glm::vec3& aPosition, const glm::vec2& aSize, const float aRotation, const glm::vec4& aColor, const float aTextureId, const float aTilingFactor, const glm::vec2 aTextureCoords[4])
    {
        GF_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), aPosition) * glm::rotate(glm::mat4(1.0f), aRotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { aSize.x,aSize.y, 1.0 });

        s_Data.myQuadVertexPtr->myPosition = transform * s_Data.myQuadVertexPosition[0];
        s_Data.myQuadVertexPtr->myColor = aColor;
        s_Data.myQuadVertexPtr->myTexCoord = aTextureCoords[0];
        s_Data.myQuadVertexPtr->myTextureID = aTextureId;
        s_Data.myQuadVertexPtr->myTilingFactor = aTilingFactor;
        ++s_Data.myQuadVertexPtr;

        s_Data.myQuadVertexPtr->myPosition = transform * s_Data.myQuadVertexPosition[1];
        s_Data.myQuadVertexPtr->myColor = aColor;
        s_Data.myQuadVertexPtr->myTexCoord = aTextureCoords[1];
        s_Data.myQuadVertexPtr->myTextureID = aTextureId;
        s_Data.myQuadVertexPtr->myTilingFactor = aTilingFactor;
        ++s_Data.myQuadVertexPtr;

        s_Data.myQuadVertexPtr->myPosition = transform * s_Data.myQuadVertexPosition[2];
        s_Data.myQuadVertexPtr->myColor = aColor;
        s_Data.myQuadVertexPtr->myTexCoord = aTextureCoords[2];
        s_Data.myQuadVertexPtr->myTextureID = aTextureId;
        s_Data.myQuadVertexPtr->myTilingFactor = aTilingFactor;
        ++s_Data.myQuadVertexPtr;

        s_Data.myQuadVertexPtr->myPosition = transform * s_Data.myQuadVertexPosition[3];
        s_Data.myQuadVertexPtr->myColor = aColor;
        s_Data.myQuadVertexPtr->myTexCoord = aTextureCoords[3];
        s_Data.myQuadVertexPtr->myTextureID = aTextureId;
        s_Data.myQuadVertexPtr->myTilingFactor = aTilingFactor;
        ++s_Data.myQuadVertexPtr;

        s_Data.myQuadIndexCount += 6;

        ++s_Data.myStatistics.myQuadCounts;
    }

    void Renderer2D::FillQuadVertexBuffer(const glm::mat4& aTransform, const glm::vec4& aColor, const float aTextureId, const float aTilingFactor, const glm::vec2 aTextureCoords[4])
    {
        GF_PROFILE_FUNCTION();

        s_Data.myQuadVertexPtr->myPosition = aTransform * s_Data.myQuadVertexPosition[0];
        s_Data.myQuadVertexPtr->myColor = aColor;
        s_Data.myQuadVertexPtr->myTexCoord = aTextureCoords[0];
        s_Data.myQuadVertexPtr->myTextureID = aTextureId;
        s_Data.myQuadVertexPtr->myTilingFactor = aTilingFactor;
        ++s_Data.myQuadVertexPtr;

        s_Data.myQuadVertexPtr->myPosition = aTransform * s_Data.myQuadVertexPosition[1];
        s_Data.myQuadVertexPtr->myColor = aColor;
        s_Data.myQuadVertexPtr->myTexCoord = aTextureCoords[1];
        s_Data.myQuadVertexPtr->myTextureID = aTextureId;
        s_Data.myQuadVertexPtr->myTilingFactor = aTilingFactor;
        ++s_Data.myQuadVertexPtr;

        s_Data.myQuadVertexPtr->myPosition = aTransform * s_Data.myQuadVertexPosition[2];
        s_Data.myQuadVertexPtr->myColor = aColor;
        s_Data.myQuadVertexPtr->myTexCoord = aTextureCoords[2];
        s_Data.myQuadVertexPtr->myTextureID = aTextureId;
        s_Data.myQuadVertexPtr->myTilingFactor = aTilingFactor;
        ++s_Data.myQuadVertexPtr;

        s_Data.myQuadVertexPtr->myPosition = aTransform * s_Data.myQuadVertexPosition[3];
        s_Data.myQuadVertexPtr->myColor = aColor;
        s_Data.myQuadVertexPtr->myTexCoord = aTextureCoords[3];
        s_Data.myQuadVertexPtr->myTextureID = aTextureId;
        s_Data.myQuadVertexPtr->myTilingFactor = aTilingFactor;
        ++s_Data.myQuadVertexPtr;

        s_Data.myQuadIndexCount += 6;

        ++s_Data.myStatistics.myQuadCounts;
    }

    float Renderer2D::GetTextureIndex(const Ref<Texture2D>& aTexture)
    {
        GF_PROFILE_FUNCTION();

        float textureIndex = 0.0f;

        for (uint32_t i = 0; i < s_Data.myTextureSlotIndex; i++)
        {
            if (*s_Data.myTextureSlots[i].get() == *aTexture.get())
            {
                textureIndex = static_cast<float>(i);
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = static_cast<float>(s_Data.myTextureSlotIndex);
            s_Data.myTextureSlots[s_Data.myTextureSlotIndex] = aTexture;
            ++s_Data.myTextureSlotIndex;
        }

        return textureIndex;
    }

    void Renderer2D::DrawQuad(const glm::vec2& aPosition, const glm::vec2& aSize, const glm::vec4& aColor)
    {
        DrawQuad(glm::vec3(aPosition, 0), aSize, aColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& aPosition, const glm::vec2& aSize, const glm::vec4& aColor)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), aPosition) * glm::rotate(glm::mat4(1.0f), 0.0f, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { aSize.x,aSize.y, 1.0 });
        DrawQuad(transform, aColor);
    }

    void Renderer2D::DrawQuad(const glm::vec2& aPosition, const glm::vec2& aSize, const Ref<Texture2D>& aTexture, const float aTilingFactor, const glm::vec4& aColor)
    {
        DrawQuad(glm::vec3(aPosition, 0), aSize, aTexture, aTilingFactor, aColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& aPosition, const glm::vec2& aSize, const Ref<Texture2D>& aTexture, const float aTilingFactor, const glm::vec4& aColor)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), aPosition) * glm::rotate(glm::mat4(1.0f), 0.0f, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { aSize.x,aSize.y, 1.0 });
        DrawQuad(transform, aTexture, aTilingFactor, aColor);
    }

    void Renderer2D::DrawQuad(const glm::vec2& aPosition, const glm::vec2& aSize, const Ref<SubTexture2D>& aSubTexture, const float aTilingFactor, const glm::vec4& aColor)
    {
        DrawQuad(glm::vec3(aPosition, 0), aSize, aSubTexture, aTilingFactor, aColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& aPosition, const glm::vec2& aSize, const Ref<SubTexture2D>& aSubTexture, const float aTilingFactor, const glm::vec4& aColor)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), aPosition) * glm::rotate(glm::mat4(1.0f), 0.0f, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { aSize.x,aSize.y, 1.0 });
        DrawQuad(transform, aSubTexture, aTilingFactor, aColor);
    }

    void Renderer2D::DrawQuad(const glm::vec2& aPosition, const glm::vec2& aSize, const float aRotation, const glm::vec4& aColor)
    {
        DrawQuad(glm::vec3(aPosition, 0), aSize, aRotation, aColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& aPosition, const glm::vec2& aSize, const float aRotation, const glm::vec4& aColor)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), aPosition) * glm::rotate(glm::mat4(1.0f), aRotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { aSize.x,aSize.y, 1.0 });
        DrawQuad(transform, aColor);
    }

    void Renderer2D::DrawQuad(const glm::vec2& aPosition, const glm::vec2& aSize, const float aRotation, const Ref<Texture2D>& aTexture, const float aTilingFactor, const glm::vec4& aColor)
    {
        DrawQuad(glm::vec3(aPosition, 0), aSize, aRotation, aTexture, aTilingFactor, aColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& aPosition, const glm::vec2& aSize, const float aRotation, const Ref<Texture2D>& aTexture, const float aTilingFactor, const glm::vec4& aColor)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), aPosition) * glm::rotate(glm::mat4(1.0f), aRotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { aSize.x,aSize.y, 1.0 });
        DrawQuad(transform, aTexture, aTilingFactor, aColor);
    }

    void Renderer2D::DrawQuad(const glm::vec2& aPosition, const glm::vec2& aSize, const float aRotation, const Ref<SubTexture2D>& aSubTexture, const float aTilingFactor, const glm::vec4& aColor)
    {
        DrawQuad(glm::vec3(aPosition, 0), aSize, aRotation, aSubTexture, aTilingFactor, aColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& aPosition, const glm::vec2& aSize, const float aRotation, const Ref<SubTexture2D>& aSubTexture, const float aTilingFactor, const glm::vec4& aColor)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), aPosition) * glm::rotate(glm::mat4(1.0f), aRotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { aSize.x,aSize.y, 1.0 });
        DrawQuad(transform, aSubTexture, aTilingFactor, aColor);
    }

    void Renderer2D::DrawQuad(const glm::mat4& aTransform, const glm::vec4& aColor)
    {
        GF_PROFILE_FUNCTION();

        if (s_Data.myQuadIndexCount >= Renderer2DData::MaxIndices)
            FlushAndReset();

        constexpr float textureIndex = 0.0f; //White texture
        constexpr float tilingFactor = 1.0f;

        FillQuadVertexBuffer(aTransform, aColor, textureIndex, tilingFactor, myDefaultTextureCoords);
    }

    void Renderer2D::DrawQuad(const glm::mat4& aTransform, const Ref<Texture2D>& aTexture, const float aTilingFactor, const glm::vec4& aColor)
    {
        GF_PROFILE_FUNCTION();

        if (s_Data.myQuadIndexCount >= Renderer2DData::MaxIndices)
            FlushAndReset();

        const float textureIndex = GetTextureIndex(aTexture);
        FillQuadVertexBuffer(aTransform, aColor, textureIndex, aTilingFactor, myDefaultTextureCoords);
    }

    void Renderer2D::DrawQuad(const glm::mat4& aTransform, const Ref<SubTexture2D>& aSubTexture, const float aTilingFactor, const glm::vec4& aColor)
    {
        GF_PROFILE_FUNCTION();

        if (s_Data.myQuadIndexCount >= Renderer2DData::MaxIndices)
            FlushAndReset();

        const float textureIndex = GetTextureIndex(aSubTexture->GetTexture());
        FillQuadVertexBuffer(aTransform, aColor, textureIndex, aTilingFactor, aSubTexture->GetTextureCoordinates());
    }

    void Renderer2D::ResetStats()
    {
        memset(&s_Data.myStatistics, 0, sizeof(Statistics));
    }

    Renderer2D::Statistics Renderer2D::GetStats()
    {
        return s_Data.myStatistics;
    }
}