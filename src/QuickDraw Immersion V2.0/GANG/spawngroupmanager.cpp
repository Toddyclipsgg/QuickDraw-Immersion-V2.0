#include "..\header.h"
#include "spawngroupmanager.h"

#pragma warning(disable:4267)
// Fun��o que executa as fun��es de forma aleat�ria sem repeti��o
void spawnGroupManager() {
    // WAIT(300000);
    WAIT(60000);

    std::vector<int> group = { 1, 2, 3};  // �ndices das fun��es

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