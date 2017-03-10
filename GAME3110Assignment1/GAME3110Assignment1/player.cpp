#include "player.h"
#include <stdio.h>

Player::Player() {
    m_isMaster = false;
    m_hasSpecial = true;
    m_health = 500.0f;
    m_speed = 3.0f;
    m_mana = 50.0f;
    m_power = 60.0f;
    m_heal = 20.0f;
    m_currentSpeed = 0.0f;
    m_multiplier = 2.5f;
}

Player::~Player() {

}

void Player::Heal() {
    m_health += m_heal;
}

void Player::Attack(std::vector<Player*> m_players) {
    for each (Player* player in m_players) {
        if (player->GetNetworkID() != this->GetNetworkID()) {
            player->TakeHit(this->m_power);
        }
    }
}

void Player::PowerUp(Power m_powerUp) {
    if (m_mana > 10.0f) {
        m_mana -= 10.0f;
        switch (m_powerUp) {
        case 0:
            m_speed *= m_multiplier;
            break;
        case 1:
            m_power *= m_multiplier;
            break;
        case 2:
            m_heal *= m_multiplier;
            break;
        default:
            printf("Invalid powerUp enum");
            break;
        }
    }
}

void Player::TakeHit(float amount) {
    m_health -= amount;
}

float Player::GetHealth() {
    return m_health;
}

void Player::Update() {
    static float fixedTime = 0.016f;
    if (m_currentSpeed < 100.0f) {
        m_currentSpeed += fixedTime;
    }
}

void Player::DisplayStats() {
    printf("********************************************************\n");
    if (m_isMaster) {
        printf("***********************PLAYER STATS***********************\n");
    } else {
        printf("***********************REPLICA STATS***********************\n");
    }
    printf("NETWORKID: %i\n", this->GetNetworkID());
    printf("HEALTH: %.2f\n", m_health);
    printf("MANA: %.2f\n", m_mana);
    printf("CURRENT SPEED: %.2f\n", m_currentSpeed);
    printf("********************************************************\n");
}