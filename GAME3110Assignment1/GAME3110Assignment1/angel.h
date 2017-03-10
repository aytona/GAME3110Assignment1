#pragma once

#include "NetworkIDManager.h"
#include <string>
#include <vector>
#include "player.h"

class Angel : public Player {
public:
    Angel();
    ~Angel();

    void Special();
};