#include "fallen.h"

Fallen::Fallen() {
    m_isMaster = false;
    m_hasSpecial = true;
    m_health = 800.0f;
    m_speed = 1.0f;
    m_mana = 40.0f;
    m_power = 100.0f;
    m_heal = 40.0f;
    m_currentSpeed = 0.0f;
    m_multiplier = 1.5f;
}

Fallen::~Fallen() {

}

void Fallen::Special() {
    printf("*******************SPECIAL!********************\n");
    printf("DIVINE POWER");
    m_power *= 5.0f;
}