#include "precompile.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

namespace Framework
{
    OpenGLTexture2D::OpenGLTexture2D(const uint32_t aWidth, const uint32_t aHeight)
        : myWidth(aWidth), myHeight(aHeight), myRendererID(0), myPath("")
    {
        GF_PROFILE_FUNCTION();

        myInternalFormat = GL_RGBA8;
        myDataFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &myRendererID);
        glTextureStorage2D(myRendererID, 1, myInternalFormat, myWidth, myHeight);

        glTextureParameteri(myRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(myRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(myRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(myRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& aPath)
        : myWidth(0), myHeight(0), myRendererID(0), myPath(aPath)
    {
        GF_PROFILE_FUNCTION();

        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = nullptr;
        {
            GF_PROFILE_SCOPE("stbi_load -> OpenGLTextur2D::OpenGLTexture2D(const std::string&)");
            data = stbi_load(aPath.c_str(), &width, &height, &channels, 0);
        }
        GF_CORE_ASSERT(data, "Failed to load image!");
        myWidth = width;
        myHeight = height;

        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        myInternalFormat = internalFormat;
        myDataFormat = dataFormat;

        GF_CORE_ASSERT(internalFormat & dataFormat, "Format not supported.");

        glCreateTextures(GL_TEXTURE_2D, 1, &myRendererID);
        glTextureStorage2D(myRendererID, 1, internalFormat, myWidth, myHeight);

        glTextureParameteri(myRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(myRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(myRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(myRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(myRendererID, 0, 0, 0, myWidth, myHeight, dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        GF_PROFILE_FUNCTION();

        glDeleteTextures(1, &myRendererID);
    }

    void OpenGLTexture2D::SetData(void* aData, uint32_t aSize)
    {
        GF_PROFILE_FUNCTION();

        uint32_t bpp = myDataFormat == GL_RGBA ? 4 : 3;
        GF_CORE_ASSERT(aSize == (myWidth * myHeight * bpp), "Data must be entire textur!");
        glTextureSubImage2D(myRendererID, 0, 0, 0, myWidth, myHeight, myDataFormat, GL_UNSIGNED_BYTE, aData);
    }

    void OpenGLTexture2D::Bind(uint32_t aSlot) const
    {
        GF_PROFILE_FUNCTION();

        glBindTextureUnit(aSlot, myRendererID);
    }
}