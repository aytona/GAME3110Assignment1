#pragma once

#include "NetworkIDManager.h"
#include <string>
#include <vector>

enum Power {
    speed = 0,
    power = 1,
    heal = 2,
};

class Player : public RakNet::NetworkIDObject {
public:
    Player();
    ~Player();

    // Commands
    void Heal();        // Heals self for a set amount
    void Attack(std::vector<Player*> m_players);      // Attacks everybody
    void PowerUp();   // Powerup a certain stat for a limited amount of time
    void TakeHit(float amount);     // When the player gets hit

    float GetHealth();
    float GetCurrentSpeed();

    virtual void Special();

    void Update();
    void DisplayStats();
    void SetIsMaster(bool isMaster) { m_isMaster = isMaster; }
    bool IsMaster() const { return m_isMaster; }

protected:
    bool m_isMaster;
    bool m_hasSpecial;

    float m_health;
    float m_speed;
    float m_mana;
    float m_power;
    
    float m_heal;
    float m_currentSpeed;
    float m_multiplier;
};