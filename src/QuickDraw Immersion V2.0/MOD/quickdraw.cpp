#include "..\header.h"

void holsterWeaponAndShoot() {

    // Desativa durante missão
    if (MISC::GET_MISSION_FLAG()) {
        return;
    }

    // Obtém o ped do jogador
    Ped ped = PLAYER::PLAYER_PED_ID();

    // Limpa quaisquer tarefas anteriores que possam interferir
    TASK::CLEAR_PED_TASKS(ped, false, false);

    // Verifica se o ped está com a arma no coldre (holster)
    if (WEAPON::_IS_PED_CURRENT_WEAPON_HOLSTERED(ped)) {
        logMessage("Gun was holstered. Automatically equipping...");

        // Equipar automaticamente a arma do slot 3
        WEAPON::_SET_FORCE_AUTO_EQUIP(ped, true);
    }

    // Aguardar até que o estado de coldre não esteja mais mudando ou um tempo limite de 2 segundos seja atingido
    int waitTime = 0;
    while (WEAPON::_IS_WEAPON_HOLSTER_STATE_CHANGING(ped) && waitTime < 2000) {
        WAIT(50);  // Verifica a cada 50 ms
        waitTime += 50;
    }

    // Verifica se o ped está com a arma sacada e pronto para disparar
    if (!WEAPON::_IS_WEAPON_HOLSTER_STATE_CHANGING(ped)) {
        logMessage("Weapon successfully drawn. Preparing to fire...");

        // Simular dois cliques do botão esquerdo do mouse (ataque)
        PAD::SET_CONTROL_VALUE_NEXT_FRAME(0, 0x07CE1E61, 1.0f);  // 1º Clique do mouse esquerdo (ataque)
        WAIT(225);  // Espera 225 ms entre os disparos (old 400 ms)

        PAD::SET_CONTROL_VALUE_NEXT_FRAME(0, 0x07CE1E61, 1.0f);  // 2º Clique do mouse esquerdo (ataque)
    }
    else {
        logMessage("Failed to equip weapon or cooldown exceeded.");
    }
}

void checkKeyPress() {
    // Verifica se a tecla "X" foi pressionada
    if (IsKeyJustUp(0x58)) {  // 0x58 é o código virtual para a tecla "X"
        logMessage("Tecla 'X' foi pressionada.");
        holsterWeaponAndShoot();
    }
}

