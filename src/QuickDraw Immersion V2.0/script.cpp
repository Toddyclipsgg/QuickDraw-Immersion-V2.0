#include "header.h"

#define DEBUG_COORD 1

std::vector<Entity> globalEntityList;
std::vector<Entity> vehicleEntityList;

#pragma warning(disable:4326)
// Fun��o principal chamada a cada frame
void main() {
    startLogThread();

    while (true) {

        spawnGroupManager();
        WAIT(0);
    }
}
#pragma warning(default:4326)

// Fun��o da thread adicional
void ScriptMain2() {
    Ped player = PLAYER::PLAYER_PED_ID();
    printStartMod();

    while (true) {

        if (IsKeyJustUp(0x34)) {
            OdriscollsVehicleSpawn();
        }

#if DEBUG_COORD
        DisplayPlayerCoordinatesPanel();
#endif // DEBUG_COORD

        startmenu();
        checkKeyPress();
        handleTabKey();
        removeAimAssist(player);
        setAccuracyTo100(player);
        WAIT(0); // Espera 1 segundo entre cada chamada para n�o sobrecarregar a execu��o
    }
}

// Fun��o da thread adicional
void ScriptMain3() {

    while (true) {

        DeleteEntitiesOnCondition(globalEntityList, 100.0f, true); // Chama a fun��o de deletar entidades
        WAIT(0); // Espera 1 segundo entre cada chamada para n�o sobrecarregar a execu��o
    }
}

#pragma warning(disable:4996)
// Fun��o de entrada do script
void ScriptMain() {
    srand(GetTickCount());

#if INJECTION_CONSOLE_ENABLE
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
#endif // INJECTION_CONSOLE_ENABLE
    initLog();
    main();
    closeLog();
}
#pragma warning(default:4996)