#pragma once


namespace Framework
{
    enum class ShaderDataType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType aType)
    {
        switch (aType)
        {
        case Framework::ShaderDataType::Float:
        case Framework::ShaderDataType::Int:
            return 4;
        case Framework::ShaderDataType::Float2:
        case Framework::ShaderDataType::Int2:
            return 4*2;
        case Framework::ShaderDataType::Float3:
        case Framework::ShaderDataType::Int3:
            return 4*3;
        case Framework::ShaderDataType::Float4:
        case Framework::ShaderDataType::Int4:
            return 4*4;
        case Framework::ShaderDataType::Mat3:
            return 4 * 3 * 3;
        case Framework::ShaderDataType::Mat4:
            return 4 * 4 * 4;
        case Framework::ShaderDataType::Bool:
            return 1;
        }

        GF_CORE_ASSERT(false, "Unkown ShaderDataType!");
        return 0;
    }

    struct BufferElement
    {
        std::string myName;
        ShaderDataType myType;
        uint32_t mySize;
        size_t myOffset;
        bool myNormalized;

        BufferElement() : myName("Uninitalized"), myType(ShaderDataType::None), mySize(0), myOffset(0), myNormalized(false) {}
        BufferElement(ShaderDataType aType, const std::string& aName, bool aNormalized = false)
            : myName(aName), myType(aType), mySize(ShaderDataTypeSize(aType)), myOffset(0), myNormalized(aNormalized)
        {

        }

        uint32_t GetComponentCount() const
        {
            switch (myType)
            {
            case Framework::ShaderDataType::Float:
            case Framework::ShaderDataType::Int:
            case Framework::ShaderDataType::Bool:
                return 1;
            case Framework::ShaderDataType::Float2:
            case Framework::ShaderDataType::Int2:
                return 2;
            case Framework::ShaderDataType::Float3:
            case Framework::ShaderDataType::Int3:
                return 3;
            case Framework::ShaderDataType::Float4:
            case Framework::ShaderDataType::Int4:
                return 4;
            case Framework::ShaderDataType::Mat3:
                return 3 * 3;
            case Framework::ShaderDataType::Mat4:
                return 4 * 4;
            }

            GF_CORE_ASSERT(false, "Unkown ShaderDataType!");
            return 0;
        }
    };

    class BufferLayout
    {
        using BufferElements = std::vector<BufferElement>;
    public:
        BufferLayout(): myStride(0) {}
        BufferLayout(const std::initializer_list<BufferElement>& aElements) 
            : myElements(aElements), myStride(0)
        {
            CalculateOffsetsAndStride();
        }

        inline const BufferElements& GetElements() const { return myElements; }
        inline const uint32_t& GetStride() const { return myStride; }

        BufferElements::iterator begin() { return myElements.begin(); }
        BufferElements::iterator end() { return myElements.end(); }
        BufferElements::const_iterator begin() const { return myElements.begin(); }
        BufferElements::const_iterator end() const { return myElements.end(); }
    private:
        void CalculateOffsetsAndStride()
        {
            size_t offset = 0;
            myStride = 0;
            for (auto& element : myElements)
            {
                element.myOffset = offset;
                offset += element.mySize;
                myStride += element.mySize;
            }
        }
    private:
        BufferElements myElements;
        uint32_t myStride;
    };

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetData(const void* aData, uint32_t aSize) = 0;

        virtual void SetLayout(const BufferLayout& aLayout) = 0;
        virtual const BufferLayout& GetLayout() = 0;

        static Ref<VertexBuffer> Create(uint32_t aSize);
        static Ref<VertexBuffer> Create(float* aVertices, uint32_t aSize);
    };

    //Currently only supports 32-bit buffer
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;
    
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0;

        static Ref<IndexBuffer> Create(uint32_t* aIndices, uint32_t aCount);
    };
}