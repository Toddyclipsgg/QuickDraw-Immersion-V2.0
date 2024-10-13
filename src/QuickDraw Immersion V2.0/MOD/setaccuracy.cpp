#include "..\header.h"

// Função para desativar a assistência de mira
void removeAimAssist(Player player) {
    PED::SET_PED_CAN_USE_AUTO_CONVERSATION_LOOKAT(player, false);
}

// Função para garantir 100% de precisão
void setAccuracyTo100(Ped ped) {
        PED::SET_PED_ACCURACY(ped, 100); // Configura precisão para 100%
        PED::SET_PED_SHOOT_RATE(ped, 1000); // Configura a taxa de disparo máxima
}
