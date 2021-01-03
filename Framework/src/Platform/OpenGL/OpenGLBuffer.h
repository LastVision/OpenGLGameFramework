#pragma once

#include "Framework/Renderer/Buffer.h"

namespace Framework
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(uint32_t aSize);
        OpenGLVertexBuffer(float* aVertices, uint32_t aSize);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetData(const void* aData, uint32_t aSize) override;

        virtual void SetLayout(const BufferLayout& aLayout) override { myLayout = aLayout; }
        virtual const BufferLayout& GetLayout() override { return myLayout; }
    private:
        uint32_t myRendererID;
        BufferLayout myLayout;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* aIndices, uint32_t aCount);
        virtual ~OpenGLIndexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual uint32_t GetCount() const override { return myCount; }

    private:
        uint32_t myRendererID;
        uint32_t myCount;
    };
}