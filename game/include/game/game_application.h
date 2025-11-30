#pragma once

#include "engine/core/application.h"
#include "game/components/profile.h"
#include <memory>

namespace game
{
class GameApplication : public engine::Application
{
  private:
    std::shared_ptr<Profile> m_loadedProfile;
    static GameApplication* s_GameApplication;

  public:
    explicit GameApplication(const engine::ApplicationSpecification& specification);

    void SetProfile(std::shared_ptr<Profile> profile);
    std::shared_ptr<Profile> GetProfile();

    static GameApplication& Get();
};
} // namespace game
