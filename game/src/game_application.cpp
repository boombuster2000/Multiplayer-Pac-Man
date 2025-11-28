#include "game/game_application.h"
#include <cassert>

namespace game
{

GameApplication* GameApplication::s_GameApplication = nullptr;

GameApplication::GameApplication(const engine::ApplicationSpecification& specification)
    : engine::Application(specification)
{
    s_GameApplication = this;
}

void GameApplication::SetProfile(std::shared_ptr<Profile> profile)
{
    m_loadedProfile = profile;
}

std::shared_ptr<Profile> GameApplication::GetProfile()
{
    return m_loadedProfile;
}

GameApplication& GameApplication::Get()
{
    assert(s_GameApplication);
    return *s_GameApplication;
}

} // namespace game
