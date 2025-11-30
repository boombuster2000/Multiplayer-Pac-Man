#pragma once
#include <filesystem>

using namespace std::filesystem;
struct FilePaths
{

    inline static const path s_profilesDirectory = "profiles";
    inline static const path s_resourceDirectory = "resources";
    inline static const path s_boardsDirectory = s_resourceDirectory / "boards";
    inline static const path s_texturesDirectory = s_resourceDirectory / "textures";
};