#pragma once
#include "raylib.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace Core
{

class TexturesManager
{

  private:
    std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_loadedTextures;

  public:
    TexturesManager();
    ~TexturesManager();

    void AddTexture(const std::string &key, const std::string &filePath);
    std::shared_ptr<Texture2D> GetTexture(const std::string &key) const;
    void UnloadAll();

    TexturesManager(const TexturesManager &) = delete;
    TexturesManager &operator=(const TexturesManager &) = delete;
};
} // namespace Core