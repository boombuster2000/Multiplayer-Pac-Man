#include "game/utils/file_utils.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace game::file_utils
{
bool SaveJson(const nlohmann::json& json_data, const std::filesystem::path& folder,
              const std::filesystem::path& filename)
{
    const std::filesystem::path path = folder / filename;
    try
    {
        std::filesystem::create_directories(folder);

        std::ofstream file;
        file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        file.open(path);
        file << json_data.dump(4);

        std::cout << "File saved successfully to " << path << std::endl;
        return true;
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        std::cerr << "Filesystem Error: " << e.what() << std::endl;
    }
    catch (const std::ios_base::failure& e)
    {
        std::cerr << "I/O Error: Failed to write to " << path << ". Reason: " << e.what() << std::endl;
    }
    catch (const nlohmann::json::exception& e)
    {
        std::cerr << "JSON Error while saving to " << path << ": " << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "An unexpected error occurred while saving to " << path << ": " << e.what() << std::endl;
    }
    return false;
}

std::vector<nlohmann::json> ReadJsonsFromDirectory(const std::filesystem::path& directory_path)
{
    std::vector<nlohmann::json> jsons;

    std::error_code ec;
    std::filesystem::create_directories(directory_path, ec);

    if (ec)
    {
        std::cerr << "Filesystem Error: Failed to create or access directory " << directory_path
                  << ". Reason: " << ec.message() << std::endl;
        return jsons;
    }

    try
    {

        for (const auto& entry : std::filesystem::directory_iterator(directory_path))
        {
            std::string ext = entry.path().extension().string();
            std::ranges::transform(ext, ext.begin(), [](unsigned char c) { return std::tolower(c); });
            if (entry.is_regular_file() && ext == ".json")
            {
                std::ifstream f(entry.path());

                if (!f.is_open())
                {
                    std::cerr << "I/O Error: Failed to open file " << entry.path() << std::endl;
                    continue;
                }

                try
                {
                    jsons.push_back(nlohmann::json::parse(f));
                }
                catch (const nlohmann::json::exception& e)
                {
                    std::cerr << "JSON Error while parsing file " << entry.path() << ": " << e.what() << std::endl;
                    continue;
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        std::cerr << "Filesystem Error while reading directory " << directory_path << ": " << e.what() << std::endl;
    }

    return jsons;
}
} // namespace game::file_utils
