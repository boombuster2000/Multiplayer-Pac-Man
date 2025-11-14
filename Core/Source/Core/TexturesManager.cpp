#include "TexturesManager.h"
#include <stdexcept>

namespace Core
{

TexturesManager::TexturesManager() = default;

TexturesManager::~TexturesManager()
{
    UnloadAll();
}

void TexturesManager::AddTexture(const std::string &key, const std::string &filePath)
{
    if (m_loadedTextures.find(key) != m_loadedTextures.end())
        return;

    auto texture = std::make_shared<Texture2D>(LoadTexture(filePath.c_str()));
    m_loadedTextures[key] = texture;
}

std::shared_ptr<Texture2D> TexturesManager::GetTexture(const std::string &key) const
{
    auto it = m_loadedTextures.find(key);
    if (it == m_loadedTextures.end())
    {
        throw std::runtime_error("Texture not found: " + key);
    }

    return it->second;
}

void TexturesManager::UnloadAll()
{
    for (auto &[key, texturePtr] : m_loadedTextures)
    {
        if (texturePtr)
        {
            UnloadTexture(*texturePtr);
        }
    }

    m_loadedTextures.clear();
}

} // namespace Core