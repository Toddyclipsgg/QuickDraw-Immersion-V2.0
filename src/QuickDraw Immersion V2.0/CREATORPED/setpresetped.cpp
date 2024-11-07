#include "..\header.h"

bool IsValidPed(Ped ped) {
    return ENTITY::DOES_ENTITY_EXIST(ped) && !ENTITY::IS_ENTITY_DEAD(ped) && !PED::IS_PED_DEAD_OR_DYING(ped, true);
}

void personalityPed(Ped ped) {
    if (IsValidPed(ped)) {
        PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
        PED::_SET_PED_INTERACTION_PERSONALITY(ped, MISC::GET_HASH_KEY("AGGRESSIVE"));
        PED::_SET_PED_PERSONALITY(ped, 993130593);
    }
}

void ConfigurePedCommonFlags(Ped ped) {
    struct PedConfigFlag {
        int flag;
        bool value;
    };

    PedConfigFlag configFlags[] = {
        // Configurações de Interação e Combate
        {130, false}, // Permite interação de fala com o ped
        {297, true},  // Força o lock-on de interação com o ped alvo
        {301, false}, // Permite o lock-on de interação com o ped alvo
        {6, true},    // Impede que o ped influencie o nível de procurado do jogador
        {233, true},  // Configura o ped como inimigo do jogador

        // Atributos e Degradação
        {361, true},  // Ignora a degradação da arma usada pelo ped
        {279, true},  // O ped nunca deixa o grupo ao qual pertence

        // Fuga e Reações
        {442, false}, // O ped não foge de uma interação
        {154, true},  // O ped responde com o líder

        // Outras flags configuráveis
        {150, true}, // PCF_ForceBleeding (força o sangramento)
        {289, true}, // PCF_TreatDislikeAsHateWhenInCombat
        {290, true}, // PCF_TreatNonFriendlyAsHateWhenInCombat
        {336, true}, // PCF_ForceInjuredMovement (força movimento de ferido)
        {351, true}, // PCF_DisableIntimidationBackingAway
        {356, true}, // PCF_BlockRobberyInteractionEscape
        {435, true}, // PCF_AlwaysRejectPlayerRobberyAttempt
        {477, true}, // PCF_CanInteractWithPlayerEvenIfInputsDisabled
    };

    const int numFlags = sizeof(configFlags) / sizeof(configFlags[0]);
    for (int i = 0; i < numFlags; ++i) {
        PED::SET_PED_CONFIG_FLAG(ped, configFlags[i].flag, configFlags[i].value);
    }
}

void ConfigurePedInteraction(Ped ped) {
    if (IsValidPed(ped)) {
        ConfigurePedCommonFlags(ped);

        // Comportamento e Montaria
        PED::SET_PED_CONFIG_FLAG(ped, 167, true); // Força o ped a estar montado em um cavalo
        PED::SET_PED_CONFIG_FLAG(ped, 307, true); // Desativa a geração automática de montaria para o ped
    } else {
        logMessage("Ped is dead. Unable to configure.");
    }
}

void ConfigurePedVehicleInteraction(Ped ped) {
    if (IsValidPed(ped)) {
        ConfigurePedCommonFlags(ped);

        // Comportamento e Montaria
        PED::SET_PED_CONFIG_FLAG(ped, 307, true); // Desativa a geração automática de montaria para o ped
    } else {
        logMessage("Ped is dead. Unable to configure.");
    }
}
