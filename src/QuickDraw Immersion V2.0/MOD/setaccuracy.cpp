#include "..\header.h"

// Fun��o para desativar a assist�ncia de mira
void removeAimAssist(Player player) {
    PED::SET_PED_CAN_USE_AUTO_CONVERSATION_LOOKAT(player, false);
}

// Fun��o para garantir 100% de precis�o
void setAccuracyTo100(Ped ped) {
        PED::SET_PED_ACCURACY(ped, 100); // Configura precis�o para 100%
        PED::SET_PED_SHOOT_RATE(ped, 1000); // Configura a taxa de disparo m�xima
}
