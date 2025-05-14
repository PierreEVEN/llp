#include "llp/file_data.hpp"

#include <filesystem>
#include <fstream>
#include <string>

namespace Llp {
	std::string FileReaderHelper::from_path(const std::filesystem::path& path)
	{
        std::ifstream data_stream(path);
        std::string line;
        std::string loaded_data;
        while (std::getline(data_stream, line))
            loaded_data += line + "\n";
        return loaded_data;
	}
}
