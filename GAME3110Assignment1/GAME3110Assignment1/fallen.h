#pragma once

#include "NetworkIDManager.h"
#include "player.h"
#include <string>
#include <vector>

class Fallen : public Player {
public:
    Fallen();
    ~Fallen();

    virtual void Special();
};