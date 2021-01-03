#pragma once
#include "Framework/Renderer/VertexArray.h"

namespace Framework
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& aVertexBuffer) override;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& aIndexBuffer) override;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return myVertexBuffers; }
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return myIndexBuffer; }

    private:
        uint32_t myRendererID;
        std::vector<Ref<VertexBuffer>> myVertexBuffers;
        Ref<IndexBuffer> myIndexBuffer;
    };
}