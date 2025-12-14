#pragma once
#include <filesystem>
struct FilePaths
{

    inline static const std::filesystem::path s_profilesDirectory = "profiles";
    inline static const std::filesystem::path s_resourceDirectory = "resources";
    inline static const std::filesystem::path s_boardsDirectory = s_resourceDirectory / "boards";
    inline static const std::filesystem::path s_texturesDirectory = s_resourceDirectory / "textures";
    inline static const std::filesystem::path s_musicDirectory = s_resourceDirectory / "music";
};