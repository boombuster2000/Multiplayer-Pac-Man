#pragma once

#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <vector>

namespace game::file_utils
{

bool SaveJson(const nlohmann::json& json_data, const std::filesystem::path& folder,
              const std::filesystem::path& filename);

std::vector<nlohmann::json> ReadJsonsFromDirectory(const std::filesystem::path& path);

} // namespace game::file_utils
