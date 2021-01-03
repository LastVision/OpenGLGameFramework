#include "precompile.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Framework
{
    static const uint32_t s_MaxFramebufferSize = 8192;

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& aSpecification)
        : mySpecification(aSpecification)
    {
        Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        glDeleteFramebuffers(1, &myRendererID);
        glDeleteTextures(1, &myColorAttachment);
        glDeleteTextures(1, &myDepthAttachment);
    }

    void OpenGLFramebuffer::Invalidate()
    {
        if (myRendererID)
        {
            glDeleteFramebuffers(1, &myRendererID);
            glDeleteTextures(1, &myColorAttachment);
            glDeleteTextures(1, &myDepthAttachment);
        }

        glCreateFramebuffers(1, &myRendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, myRendererID);

        glCreateTextures(GL_TEXTURE_2D, 1, &myColorAttachment);
        glBindTexture(GL_TEXTURE_2D, myColorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mySpecification.Width, mySpecification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, myColorAttachment, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &myDepthAttachment);
        glBindTexture(GL_TEXTURE_2D, myDepthAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, mySpecification.Width, mySpecification.Height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, myDepthAttachment, 0);

        GF_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, myRendererID);
        glViewport(0, 0, mySpecification.Width, mySpecification.Height);
    }

    void OpenGLFramebuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Resize(const uint32_t aWidth, const uint32_t aHeight)
    {
        if (aWidth == 0 || aHeight == 0 || aWidth > s_MaxFramebufferSize || aHeight > s_MaxFramebufferSize)
        {
            GF_CORE_LOG_WARN("Attempted to resize framebuffer to {0}, {1}", aWidth, aHeight);
            return;
        }

        mySpecification.Width = aWidth;
        mySpecification.Height = aHeight;

        Invalidate();
    }
}
