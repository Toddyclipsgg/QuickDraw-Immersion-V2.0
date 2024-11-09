#include "header.h"

#define DEBUG_COORD 1

std::vector<Entity> globalEntityList;
std::vector<Entity> vehicleEntityList;

// Atualizar MainLoop
void MainLoop() {
    startLogThread();

    while (true) {
        spawnGroupManager();
        WAIT(0);
    }
}

// Atualizar AdditionalThread1
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

// Atualizar AdditionalThread2
void AdditionalThread2() {
    while (true) {
        DeleteEntitiesOnCondition(globalEntityList, 100.0f, true);
        WAIT(0);
    }
}

// Atualizar ScriptMain
void ScriptMain() {
    srand(GetTickCount());

#if INJECTION_CONSOLE_ENABLE
    AllocConsole();
    #pragma warning(disable: 4996)
    freopen("CONOUT$", "w", stdout);
    #pragma warning(default: 4996)
#endif

    initLog();
    MainLoop();
    closeLog();
}