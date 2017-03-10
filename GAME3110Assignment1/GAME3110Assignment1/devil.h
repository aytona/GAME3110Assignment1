#pragma once

#include "NetworkIDManager.h"
#include <string>
#include <vector>
#include "player.h"

class Devil : public Player {
public:
    Devil();
    ~Devil();

    virtual void Special();
};