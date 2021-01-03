#pragma once
#include "Framework/Core/Core.h"
#include <string>

namespace Framework
{
    class Texture
    {
    public:
        virtual ~Texture() = default;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual uint32_t GetRendererID() const = 0;
        
        virtual void SetData(void* aData, uint32_t aSize) = 0;

        virtual void Bind(uint32_t aSlot = 0) const = 0;

        virtual bool operator==(const Texture& other) const = 0;
    };

    class Texture2D : public Texture
    {
    public:
        static Ref<Texture2D> Create(const uint32_t aWidth, const uint32_t aHeight);
        static Ref<Texture2D> Create(const std::string& aPath);
    };
}