#pragma once
#include <filesystem>

using namespace std::filesystem;
struct FilePaths
{

    inline static const path s_ProfilesDirectory = "profiles";
    inline static const path s_resourceDirectory = "resources";
    inline static const path s_BoardsDirectory = s_resourceDirectory / "boards";
    inline static const path s_TexturesDirectory = s_resourceDirectory / "textures";
};