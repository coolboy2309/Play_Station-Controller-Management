#pragma once

#include "MatchState.h"

class GameStateManager
{
public:

    static MatchState Detect(int minute);
};