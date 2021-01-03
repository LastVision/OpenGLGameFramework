#pragma once
#include <string>
#include <optional>

namespace Framework
{
    class FileDialogs
    {
    public:
        //Opens up file dialog for opening files. Returns empty string if cancelled
        static std::optional<std::string> OpenFile(const char* filter);
        //Opens up file dialog for saving files. Returns empty string if cancelled
        static std::optional<std::string> SaveFile(const char* filter);
    };
}