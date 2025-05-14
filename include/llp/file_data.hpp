#pragma once
#include <filesystem>

namespace Llp {

    class FileReaderHelper
    {
    public:
        static std::string from_path(const std::filesystem::path& path);
    };
}