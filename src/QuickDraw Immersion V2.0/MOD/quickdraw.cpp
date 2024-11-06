#include "..\header.h"

void holsterWeaponAndShoot() {
    // Desativa durante missão
    if (MISC::GET_MISSION_FLAG()) {
        return;
    }

    Ped ped = PLAYER::PLAYER_PED_ID();
    TASK::CLEAR_PED_TASKS(ped, false, false);

    if (WEAPON::_IS_PED_CURRENT_WEAPON_HOLSTERED(ped)) {
        logMessage("Gun was holstered. Automatically equipping...");
        WEAPON::_SET_FORCE_AUTO_EQUIP(ped, true);
    }

    int waitTime = 0;
    while (WEAPON::_IS_WEAPON_HOLSTER_STATE_CHANGING(ped) && waitTime < 2000) {
        WAIT(50);
        waitTime += 50;
    }

    if (!WEAPON::_IS_WEAPON_HOLSTER_STATE_CHANGING(ped)) {
        logMessage("Weapon successfully drawn. Preparing to fire...");
        PAD::SET_CONTROL_VALUE_NEXT_FRAME(0, 0x07CE1E61, 1.0f);
        WAIT(225);
        PAD::SET_CONTROL_VALUE_NEXT_FRAME(0, 0x07CE1E61, 1.0f);
    } else {
        logMessage("Failed to equip weapon or cooldown exceeded.");
    }
}

void checkKeyPress() {
    if (IsKeyJustUp(0x58)) {
        logMessage("Tecla 'X' foi pressionada.");
        holsterWeaponAndShoot();
    }
}
