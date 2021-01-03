#pragma once
#include "Framework/Renderer/Texture.h"

#include <glad/glad.h>

namespace Framework
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(const uint32_t aWidth, const uint32_t aHeight);
        OpenGLTexture2D(const std::string& aPath);
        virtual ~OpenGLTexture2D();

        virtual uint32_t GetWidth() const override { return myWidth; }
        virtual uint32_t GetHeight() const override { return myHeight; }
        virtual uint32_t GetRendererID() const override { return myRendererID; }

        virtual void SetData(void* aData, uint32_t aSize) override;

        virtual void Bind(uint32_t aSlot = 0) const override;

        virtual bool operator==(const Texture& aOther) const override { return myRendererID == ((OpenGLTexture2D&)aOther).myRendererID; }
    private:
        std::string myPath;
        uint32_t myWidth, myHeight;
        uint32_t myRendererID;
        GLenum myInternalFormat, myDataFormat;
    };
}