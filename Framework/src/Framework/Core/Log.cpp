#include "precompile.h"
#include "Log.h"

//Sinks
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Framework
{
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::init()
    {
        // create the sinks
        std::vector<spdlog::sink_ptr> sinks;
        sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>()); // VS debug console

        // create the loggers
        s_CoreLogger = std::make_shared<spdlog::logger>("Framework", begin(sinks), end(sinks));
        spdlog::register_logger(s_CoreLogger);
        s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(sinks), end(sinks));
        spdlog::register_logger(s_ClientLogger);

        // configuration
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_CoreLogger->set_level(spdlog::level::level_enum::trace);
        s_ClientLogger->set_level(spdlog::level::level_enum::trace);
    }
}