#include "devil.h"

Devil::Devil() {
    m_isMaster = false;
    m_hasSpecial = true;
    m_health = 1200.0f;
    m_speed = 5.0f;
    m_mana = 30.0f;
    m_power = 80.0f;
    m_heal = 30.0f;
    m_currentSpeed = 0.0f;
    m_multiplier = 3.0f;
}

Devil::~Devil() {

}

void Devil::Special() {
    m_currentSpeed = 0;
    printf("*******************SPECIAL!********************\n");
    printf("DEVIOUSLY CUNNING");
    m_multiplier *= 5;
}