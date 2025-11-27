#pragma once

#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <vector>

namespace game
{
namespace file_utils
{

bool SaveJson(const nlohmann::json& json_data, const std::filesystem::path& folder,
              const std::filesystem::path& filename);

std::vector<nlohmann::json> ReadJsonsFromDirectory(const std::filesystem::path& path);

} // namespace file_utils
} // namespace game
