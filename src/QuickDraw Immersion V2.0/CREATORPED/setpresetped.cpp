#include "..\header.h"

void personalityPed(Ped ped) {

    if (ENTITY::DOES_ENTITY_EXIST(ped) && !ENTITY::IS_ENTITY_DEAD(ped)) {

        PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
        PED::_SET_PED_INTERACTION_PERSONALITY(ped, MISC::GET_HASH_KEY("AGGRESSIVE"));
        PED::_SET_PED_PERSONALITY(ped, 993130593);
    }
}

void ConfigurePedInteraction(Ped ped) {

    if (!ENTITY::IS_ENTITY_DEAD(ped) && !PED::IS_PED_DEAD_OR_DYING(ped, true)) {

        // Configura��es de Intera��o e Combate
        PED::SET_PED_CONFIG_FLAG(ped, 130, false); // Permite intera��o de fala com o ped
        PED::SET_PED_CONFIG_FLAG(ped, 297, true);  // For�a o lock-on de intera��o com o ped alvo
        PED::SET_PED_CONFIG_FLAG(ped, 301, false); // Permite o lock-on de intera��o com o ped alvo
        PED::SET_PED_CONFIG_FLAG(ped, 6, true);    // Impede que o ped influencie o n�vel de procurado do jogador
        PED::SET_PED_CONFIG_FLAG(ped, 233, true);  // Configura o ped como inimigo do jogador

        // Comportamento e Montaria
        PED::SET_PED_CONFIG_FLAG(ped, 167, true);  // For�a o ped a estar montado em um cavalo
        PED::SET_PED_CONFIG_FLAG(ped, 307, true);  // Desativa a gera��o autom�tica de montaria para o ped

        // Atributos e Degrada��o
        PED::SET_PED_CONFIG_FLAG(ped, 361, true);  // Ignora a degrada��o da arma usada pelo ped
        PED::SET_PED_CONFIG_FLAG(ped, 279, true);  // O ped nunca deixa o grupo ao qual pertence

        // Fuga e Rea��es
        PED::SET_PED_CONFIG_FLAG(ped, 442, false);  // O ped n�o foge de uma intera��o
        PED::SET_PED_CONFIG_FLAG(ped, 154, true);  // O ped responde com o l�der

        // Outras flags configur�veis (baseadas nos seus coment�rios)
        // PED::SET_PED_CONFIG_FLAG(ped, 9, true);    // PCF_KilledByStealth (morte por stealth permitida)
        PED::SET_PED_CONFIG_FLAG(ped, 150, true);  // PCF_ForceBleeding (for�a o sangramento)
        PED::SET_PED_CONFIG_FLAG(ped, 161, true);  // PCF_IsWhistling (ped pode assobiar)
        PED::SET_PED_CONFIG_FLAG(ped, 289, true);  // PCF_TreatDislikeAsHateWhenInCombat (trata "dislike" como "hate" em combate)
        PED::SET_PED_CONFIG_FLAG(ped, 290, true);  // PCF_TreatNonFriendlyAsHateWhenInCombat (trata n�o amig�veis como "hate")
        PED::SET_PED_CONFIG_FLAG(ped, 336, true);  // PCF_ForceInjuredMovement (for�a movimento de ferido)
        PED::SET_PED_CONFIG_FLAG(ped, 351, true);  // PCF_DisableIntimidationBackingAway (n�o recua sob intimida��o)
        PED::SET_PED_CONFIG_FLAG(ped, 356, true);  // PCF_BlockRobberyInteractionEscape (bloqueia fuga durante roubo)
        PED::SET_PED_CONFIG_FLAG(ped, 359, true);  // PCF_AllowInCombatInteractionLockonOnTargetPed (permitir lock-on em combate)
        // PED::SET_PED_CONFIG_FLAG(ped, 406, true);  // PCF_ForceOfferItemOnReceivingRobberyInteraction (for�a oferta de item no roubo)
        // PED::SET_PED_CONFIG_FLAG(ped, 435, true);  // PCF_AlwaysRejectPlayerRobberyAttempt (sempre rejeita tentativa de roubo do jogador)
        PED::SET_PED_CONFIG_FLAG(ped, 505, true);  // PCF_AllowRobberyWhenInjured (permitir roubo mesmo ferido)
        PED::SET_PED_CONFIG_FLAG(ped, 477, true);  // PCF_CanInteractWithPlayerEvenIfInputsDisabled (intera��o mesmo com inputs desativados)

    }
    else {
        logMessage("O Ped est� morto. N�o foi poss�vel configurar.");
    }
}
