#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace serialization
{

inline void require_object(const nlohmann::json& j, const char* type_name) {
    if (!j.is_object()) {
        throw nlohmann::json::type_error::create(302, std::string(type_name) + " must be a JSON object.", &j);
    }
}

template <typename T>
void get_required_field(const nlohmann::json& j, const char* key, T& value, const char* type_name, int error_code = 500) {
    try {
        j.at(key).get_to(value);
    } catch (const nlohmann::json::exception& e) {
        throw nlohmann::json::other_error::create(error_code, "Failed to deserialize " + std::string(type_name) + "." + std::string(key) + ": " + e.what(), &j);
    }
}

} // namespace serialization
