#include "..\header.h"
#include "spawngroupmanager.h"

// Define membro plus/free
#define MEMBER_PLUSGANG 0
#define MEMBER_FREEGANG 1

#pragma warning(disable:4267)
// Fun��o que executa as fun��es de forma aleat�ria sem repeti��o
void spawnGroupManager() {
    WAIT(150000);
    // WAIT(30000);

    // Dist�ncia m�xima permitida (100 metros)
    double maxDistance = 100.0;

    // Verificar se o jogador est� a 100 metros de qualquer uma das localiza��es
    if (MISC::GET_MISSION_FLAG() || isPlayerWithinDistance(maxDistance)) {
        logMessage("Player is within 100 meters of a camp or on a mission.");
        return;
    }
    else {
        logMessage("Player is not within 100 meters of any camp or on a mission.");
    }

#if MEMBER_PLUSGANG
std::vector<int> group = { 1, 2, 3 };  // �ndices das fun��es
#endif // MEMBER_PLUSGANG

#if MEMBER_FREEGANG
std::vector<int> group = { 1 };  // �ndices das fun��es
#endif // MEMBER_FREEGANG

    

    if (group.empty()) {
        return;
    }

    // Embaralha as fun��es

    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> dist(0, group.size() - 1);
    int indexChosen = group[dist(g)];

    // Executa as fun��es conforme a ordem embaralhada
    switch (indexChosen) {
    case 1:
        logMessage("O'driscolls starting...");
        OdriscollsMountSpawn();
        break;
    case 2:
        logMessage("Lemoyne starting...");
        LemoyneMountSpawn();
        break;
    case 3:
        logMessage("Skinner starting...");
        SkinnerMountSpawn();
        break;
    default:
        logMessage("Invalid group index.");
        break;
    }
}
#pragma warning(default:4267)