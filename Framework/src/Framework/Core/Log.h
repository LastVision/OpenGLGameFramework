#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Framework
{
    class Log
    {
    public:
        static void init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

#define GF_CORE_LOG_FATAL(...)  ::Framework::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define GF_CORE_LOG_ERROR(...)  ::Framework::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GF_CORE_LOG_WARN(...)   ::Framework::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GF_CORE_LOG_INFO(...)   ::Framework::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GF_CORE_LOG_TRACE(...)  ::Framework::Log::GetCoreLogger()->trace(__VA_ARGS__)
        
#define GF_LOG_FATAL(...)       ::Framework::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define GF_LOG_ERROR(...)       ::Framework::Log::GetClientLogger()->error(__VA_ARGS__)
#define GF_LOG_WARN(...)        ::Framework::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GF_LOG_INFO(...)        ::Framework::Log::GetClientLogger()->info(__VA_ARGS__)
#define GF_LOG_TRACE(...)       ::Framework::Log::GetClientLogger()->trace(__VA_ARGS__)