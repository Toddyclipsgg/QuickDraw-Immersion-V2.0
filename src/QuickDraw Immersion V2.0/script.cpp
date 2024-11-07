#include "header.h"

#define DEBUG_COORD 1

std::vector<Entity> globalEntityList;
std::vector<Entity> vehicleEntityList;

// Função principal chamada a cada frame
void MainLoop() {
    startLogThread();

    while (true) {
        spawnGroupManager();
        WAIT(0);
    }
}

// Função da thread adicional
void AdditionalThread1() {
    BUILTIN::SET_THIS_THREAD_PRIORITY(0);
    Ped player = PLAYER::PLAYER_PED_ID();
    printStartMod();

    while (true) {
        if (IsKeyJustUp(0x34)) {
            OdriscollsVehicleSpawn();
        }

#if DEBUG_COORD
        DisplayPlayerCoordinatesPanel();
#endif

        startmenu();
        checkKeyPress();
        handleTabKey();
        removeAimAssist(player);
        setAccuracyTo100(player);
        WAIT(0);
    }
}

// Função da thread adicional
void AdditionalThread2() {
    while (true) {
        DeleteEntitiesOnCondition(globalEntityList, 100.0f, true);
        WAIT(0);
    }
}

// Função de entrada do script
void ScriptMain() {
    srand(GetTickCount());

#if INJECTION_CONSOLE_ENABLE
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
#endif

    initLog();
    MainLoop();
    closeLog();
}
