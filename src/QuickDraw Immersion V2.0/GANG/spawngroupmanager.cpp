#include "..\header.h"
#include "spawngroupmanager.h"

// Define teste/produto
#define TEST_SPAWN 0
#define PRODUCT_FINAL 1

#pragma warning(disable:4267)

void spawnGroupManager() {
    
#if PRODUCT_FINAL
    WAIT(150000);
#endif // PRODUCT_FINAL

#if TEST_SPAWN
    WAIT(30000);
#endif // TEST_SPAWN

    // Distância máxima permitida (100 metros)
    const double maxDistance = 500.0;

    // Verificar se o jogador está a 100 metros de qualquer uma das localizações
    if (MISC::GET_MISSION_FLAG() || isPlayerWithinDistance(maxDistance)) {
        logMessage("Player is within 100 meters of a camp or on a mission.");
        return;
    } else {
        logMessage("Player is not within 100 meters of any camp or on a mission.");
    }

    std::vector<int> group;
    group = { 1, 2};  // Índices das funções

    if (group.empty()) {
        return;
    }

    // Embaralha as funções
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> dist(0, group.size() - 1);
    int indexChosen = group[dist(g)];

    // Executa as funções conforme a ordem embaralhada
    switch (indexChosen) {
    case 1:
        logMessage("O'driscolls starting...");
        OdriscollsMountSpawn();
        break;
    case 2:
        logMessage("Lemoyne starting...");
        OdriscollsVehicleSpawn();
        break;
    default:
        logMessage("Invalid group index.");
        break;
    }
}

#pragma warning(default:4267)
