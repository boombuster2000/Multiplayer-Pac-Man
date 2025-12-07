#pragma once
#include "pacman.h"
#include "player.h"
#include "player_input.h"
#include "profile.h"
#include <memory>

struct Client
{
    std::shared_ptr<Profile> profile;
    Player player;
    Pacman pacman;
    PlayerInput* input;
};