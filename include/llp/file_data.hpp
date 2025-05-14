#pragma once
#include <xstring>

namespace std::filesystem
{
	class path;
}

namespace Llp {

    class FileReaderHelper
    {
    public:
        static std::string from_path(const std::filesystem::path& path);
    };
}
