#include "angel.h"

Angel::Angel() {
    m_isMaster = false;
    m_hasSpecial = true;
    m_health = 450.0f;
    m_speed = 4.0f;
    m_mana = 100.0f;
    m_power = 70.0f;
    m_heal = 100.0f;
    m_currentSpeed = 0.0f;
    m_multiplier = 2.0f;
}

Angel::~Angel() {

}

void Angel::Special() {
    printf("*******************SPECIAL!********************\n");
    printf("DIVINE PROTECTION");
    m_health *= (m_heal * 5);
}