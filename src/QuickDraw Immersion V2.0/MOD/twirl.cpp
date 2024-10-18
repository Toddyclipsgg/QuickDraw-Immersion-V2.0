#include "..\header.h"

std::unordered_set<Hash> validpistolAndRevolver = {
    0x5B78B8DD, 0x8580C63E, 0x657065D6, 0x020D13FF, 0x4AAE5FFA, 0x9CB1415E,
    0x169F59F7, 0xDC566A38, 0xA6FE9435, 0x1EAA7376, 0xC9622757, 0x8FAE73BB,
    0xC9095426, 0x16D655F7, 0xF5E4207F, 0x49F6BE32, 0x8384D5FE, 0x3EECE288,
    0x0797FBF5, 0x481FBBC9, 0x23C706CD, 0x83DD5617, 0x514B39A1, 0x02300C65,
    0x00D427AD, 0x5B2D26B5, 0x07E945C8, 0x1731B466, 0x7BBD1FF6, 0x6DFE44AB,
    0x0A6CD7DF, 0x0247E783, 0xCFF731A2, 0xFA4B2D47, 0xD44A5A04, 0xE195D259,
    0x99496406
};

int selected_single_variation = 0;
bool isTwirlActive = false;
bool weaponDrawn = false;
bool animationRequested = false;

// Função de log para ações de animação
void logTwirlAction(const char* message) {
    logMessage(message);
}

// Verifica se a arma atual é uma pistola ou revólver válida
bool isValidWeapon(Hash weapon) {
    return validpistolAndRevolver.count(weapon) > 0;
}

// Função para verificar se a arma foi sacada e é válida
void checkWeaponDrawnSync() {
    Ped ped = PLAYER::PLAYER_PED_ID();
    Hash currentWeapon;

    if (WEAPON::GET_CURRENT_PED_WEAPON(ped, &currentWeapon, true, 0, false)) {
        weaponDrawn = isValidWeapon(currentWeapon);
    }
    else {
        weaponDrawn = false;
    }
}

// Função para executar animações de twirl
void playTwirlAnimation(int variation_num) {
    Ped ped = PLAYER::PLAYER_PED_ID();

    // Reiniciar tarefas se a animação já estiver ativa
    if (isTwirlActive) {
        TASK::CLEAR_PED_TASKS(ped, false, false);
    }

    Hash kit_emote_twirl_type = MISC::GET_HASH_KEY("KIT_EMOTE_TWIRL_GUN");
    const int emote_category = 4;

    auto it = weapon_twirl_variation_names.find(variation_num);
    if (it != weapon_twirl_variation_names.end()) {
        WEAPON::_SET_ACTIVE_GUN_SPINNING_EQUIP_KIT_EMOTE_TWIRL(ped, kit_emote_twirl_type);
        TASK::TASK_PLAY_EMOTE_WITH_HASH(ped, emote_category, 1, kit_emote_twirl_type, true, true, false, false, false);
        WEAPON::_SET_ACTIVE_GUN_SPINNING_KIT_EMOTE_TWIRL(ped, 0, MISC::GET_HASH_KEY(it->second));
        WEAPON::_SET_GUN_SPINNING_INVENTORY_SLOT_ID_ACTIVATE(ped, 0);
        isTwirlActive = true;
    }
    else {
        logTwirlAction("Invalid animation variation.");
    }
}

// Função para iniciar a verificação da arma após 0.9 segundo
void startWeaponCheck() {
    WAIT(550);  // Espera 0.5 segundo 550 ms (old 900 ms)
    checkWeaponDrawnSync();
}

// Função específica para animação com uma arma com atraso
void twirlGunSingle(int variation_num) {
    playTwirlAnimation(variation_num);
}

// Função para alternar o estado da animação com a tecla pressionada
void toggleTwirl() {

    // Desativa durante missão
    if (MISC::GET_MISSION_FLAG()) {
        return;
    }

    Ped ped = PLAYER::PLAYER_PED_ID();

    if (isTwirlActive) {
        TASK::CLEAR_PED_TASKS(ped, false, false);
        isTwirlActive = false;
    }
    else {
        startWeaponCheck();
        if (weaponDrawn) {
            animationRequested = true;
            twirlGunSingle(selected_single_variation);
        }
        else {
            logTwirlAction("Weapon is not drawn, cannot start animation.");
        }
    }
}

// Função para lidar com a tecla "TAB" pressionada
void handleTabKey() {
    if (IsKeyJustUp(0x09)) {  // "TAB" pressionado
        toggleTwirl();
    }
}
