#pragma once
#include "raylib.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace engine
{

class TexturesManager
{

  private:
    std::unordered_map<std::string_view, std::shared_ptr<Texture2D>> m_loadedTextures;

  public:
    TexturesManager();
    ~TexturesManager();

    void AddTexture(std::string_view key, std::string_view filePath);
    std::shared_ptr<Texture2D> GetTexture(std::string_view key) const;
    void UnloadAll();

    TexturesManager(const TexturesManager&) = delete;
    TexturesManager& operator=(const TexturesManager&) = delete;
};
} // namespace engine