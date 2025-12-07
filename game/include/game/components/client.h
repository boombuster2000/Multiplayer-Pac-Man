#pragma once
#include "pacman.h"
#include "player.h"
#include "player_input.h"
#include "profile.h"

struct Client
{
    Profile* profile;
    Player* player;
    Pacman pacman;
    PlayerInput* input;
};