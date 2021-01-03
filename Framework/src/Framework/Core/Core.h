#pragma once
#include <memory>

#ifdef GF_DEBUG
    #define GF_ENABLE_ASSERTS
#endif

#ifdef GF_ENABLE_ASSERTS
    #define GF_ASSERT(x, ...) { if(!(x)) { GF_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define GF_CORE_ASSERT(x, ...) { if(!(x)) { GF_CORE_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define GF_ASSERT(x, ...)
    #define GF_CORE_ASSERT(x, ...)
#endif

#define BIT(x) 1 << x

#define GF_BIND_EVENT_FN(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

namespace Framework
{
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}