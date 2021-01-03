#pragma once

#include "Framework/Renderer/Framebuffer.h"

namespace Framework
{
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& aSpecification);
        virtual ~OpenGLFramebuffer();

        void Invalidate();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void Resize(const uint32_t aWidth, const uint32_t aHeight) override;

        virtual uint32_t GetColorAttachmentRendererID() const override { return myColorAttachment; }

        virtual const FramebufferSpecification& GetSpecification() const override { return mySpecification; }
    private:
        uint32_t myRendererID = 0;
        uint32_t myColorAttachment = 0;
        uint32_t myDepthAttachment = 0;
        FramebufferSpecification mySpecification;
    };
}