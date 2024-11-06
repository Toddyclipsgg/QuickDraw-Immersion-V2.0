#include "..\header.h"

void personalityPed(Ped ped) {
    if (ENTITY::DOES_ENTITY_EXIST(ped) && !ENTITY::IS_ENTITY_DEAD(ped)) {
        PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
        PED::_SET_PED_INTERACTION_PERSONALITY(ped, MISC::GET_HASH_KEY("AGGRESSIVE"));
        PED::_SET_PED_PERSONALITY(ped, 993130593);
    }
}

void ConfigurePedCommonFlags(Ped ped) {
    // Configurações de Interação e Combate
    PED::SET_PED_CONFIG_FLAG(ped, 130, false); // Permite interação de fala com o ped
    PED::SET_PED_CONFIG_FLAG(ped, 297, true);  // Força o lock-on de interação com o ped alvo
    PED::SET_PED_CONFIG_FLAG(ped, 301, false); // Permite o lock-on de interação com o ped alvo
    PED::SET_PED_CONFIG_FLAG(ped, 6, true);    // Impede que o ped influencie o nível de procurado do jogador
    PED::SET_PED_CONFIG_FLAG(ped, 233, true);  // Configura o ped como inimigo do jogador

    // Atributos e Degradação
    PED::SET_PED_CONFIG_FLAG(ped, 361, true);  // Ignora a degradação da arma usada pelo ped
    PED::SET_PED_CONFIG_FLAG(ped, 279, true);  // O ped nunca deixa o grupo ao qual pertence

    // Fuga e Reações
    PED::SET_PED_CONFIG_FLAG(ped, 442, false);  // O ped não foge de uma interação
    PED::SET_PED_CONFIG_FLAG(ped, 154, true);   // O ped responde com o líder

    // Outras flags configuráveis
    PED::SET_PED_CONFIG_FLAG(ped, 150, true);  // PCF_ForceBleeding (força o sangramento)
    PED::SET_PED_CONFIG_FLAG(ped, 289, true);  // PCF_TreatDislikeAsHateWhenInCombat (trata "dislike" como "hate" em combate)
    PED::SET_PED_CONFIG_FLAG(ped, 290, true);  // PCF_TreatNonFriendlyAsHateWhenInCombat (trata não amigáveis como "hate")
    PED::SET_PED_CONFIG_FLAG(ped, 336, true);  // PCF_ForceInjuredMovement (força movimento de ferido)
    PED::SET_PED_CONFIG_FLAG(ped, 351, true);  // PCF_DisableIntimidationBackingAway (não recua sob intimidação)
    PED::SET_PED_CONFIG_FLAG(ped, 356, true);  // PCF_BlockRobberyInteractionEscape (bloqueia fuga durante roubo)
    PED::SET_PED_CONFIG_FLAG(ped, 435, true);  // PCF_AlwaysRejectPlayerRobberyAttempt (sempre rejeita tentativa de roubo do jogador)
    PED::SET_PED_CONFIG_FLAG(ped, 477, true);  // PCF_CanInteractWithPlayerEvenIfInputsDisabled (interação mesmo com inputs desativados)
}

void ConfigurePedInteraction(Ped ped) {
    if (!ENTITY::IS_ENTITY_DEAD(ped) && !PED::IS_PED_DEAD_OR_DYING(ped, true)) {
        ConfigurePedCommonFlags(ped);

        // Comportamento e Montaria
        PED::SET_PED_CONFIG_FLAG(ped, 167, true);  // Força o ped a estar montado em um cavalo
        PED::SET_PED_CONFIG_FLAG(ped, 307, true);  // Desativa a geração automática de montaria para o ped
    } else {
        logMessage("Ped is dead. Unable to configure.");
    }
}

void ConfigurePedVehicleInteraction(Ped ped) {
    if (!ENTITY::IS_ENTITY_DEAD(ped) && !PED::IS_PED_DEAD_OR_DYING(ped, true)) {
        ConfigurePedCommonFlags(ped);

        // Comportamento e Montaria
        PED::SET_PED_CONFIG_FLAG(ped, 307, true);  // Desativa a geração automática de montaria para o ped
    } else {
        logMessage("Ped is dead. Unable to configure.");
    }
}

//COMPANION::_0xD730281E496621FB(func_176(uParam0, 1), joaat("support"));
