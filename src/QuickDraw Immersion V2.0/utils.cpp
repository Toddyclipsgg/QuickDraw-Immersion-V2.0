#include "header.h"

// Fun��o para calcular dist�ncia 3D entre o ped e as coordenadas fornecidas
float CalcularDistancia(Vector3 pedCoords, Vector3 destinoCoords) {
    return BUILTIN::VDIST(pedCoords.x, pedCoords.y, pedCoords.z, destinoCoords.x, destinoCoords.y, destinoCoords.z);
}

// Fun��o para calcular a dist�ncia entre dois pontos no espa�o 3D
float calcularDistanciaObstacle(Vector3 pos1, Vector3 pos2) {
    return BUILTIN::SQRT(BUILTIN::POW(pos2.x - pos1.x, 2) + BUILTIN::POW(pos2.y - pos1.y, 2) + BUILTIN::POW(pos2.z - pos1.z, 2));
}

// Fun��o para calcular a dist�ncia horizontal entre duas entidades (2D)
float distanceBetweenEntitiesHor(Entity entity1, Entity entity2)
{
    // Obtendo as coordenadas das entidades
    Vector3 pos1 = ENTITY::GET_ENTITY_COORDS(entity1, 1, 0);
    Vector3 pos2 = ENTITY::GET_ENTITY_COORDS(entity2, 1, 0);
    // Calculando a dist�ncia horizontal entre as entidades
    float distance = MISC::GET_DISTANCE_BETWEEN_COORDS(pos1.x, pos1.y, 0, pos2.x, pos2.y, 0, 1);
    logMessage("2D distance calculated successfully.");
    return distance;
}

// Fun��o para excluir entidades com base em condi��es espec�ficas
bool DeleteEntitiesOnCondition(std::vector<Entity>& entities, float maxDistance, bool deleteOnPlayerDeath) {
    Player playerPed = PLAYER::PLAYER_PED_ID();
    if (!playerPed) return false;  // Verifica��o inicial de valida��o do jogador

    // Checa se o jogador est� morto e deleteOnPlayerDeath est� ativado
    if (deleteOnPlayerDeath && ENTITY::IS_ENTITY_DEAD(playerPed)) {
        logMessage("Player is dead. Deleting all entities.");
        for (Entity& entity : entities) {
            if (ENTITY::DOES_ENTITY_EXIST(entity)) {
                ENTITY::DELETE_ENTITY(&entity);
                // Certifica que ve�culos e cavalos sejam exclu�dos tamb�m
                DeleteVehiclesAndHorses(vehicleEntityList, 250.0f, true);
            }
        }
        entities.clear();  // Limpar a lista de entidades
        return true;
    }

    Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);
    static std::unordered_map<Entity, int> pedStopTimes;

    for (auto it = entities.begin(); it != entities.end();) {
        if (!ENTITY::DOES_ENTITY_EXIST(*it)) {
            it = entities.erase(it);
            continue;
        }

        Vector3 entityCoords = ENTITY::GET_ENTITY_COORDS(*it, true, false);
        float distance = BUILTIN::VDIST2(playerCoords.x, playerCoords.y, playerCoords.z, entityCoords.x, entityCoords.y, entityCoords.z);

        if (distance > maxDistance * maxDistance || ENTITY::IS_ENTITY_DEAD(*it)) {
            logMessage("Marking entity as no longer needed: out of range or dead.");
            ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&(*it));
            DeleteVehiclesAndHorses(vehicleEntityList, 250.0f, true); // Chamada para deletar ve�culos e cavalos
            pedStopTimes.erase(*it);
            it = entities.erase(it);
            continue;
        }

        if (PED::IS_PED_STOPPED(*it)) {
            if (CheckEntityObstacles(*it)) {
                ++it;  // Pular exclus�o se houver obst�culos
                continue;
            }

            if (HandleEntityInteractions(*it, playerPed)) {
                ++it;  // Pular exclus�o se estiver interagindo com o jogador
                continue;
            }

            if (HandleCombatStatus(*it, playerPed)) {
                ++it;  // Pular exclus�o se estiver em combate
                continue;
            }

            if (pedStopTimes.find(*it) == pedStopTimes.end()) {
                pedStopTimes[*it] = BUILTIN::TIMERA();
                logMessage("Ped stopped, starting timer.");
            }
            else if (BUILTIN::TIMERA() - pedStopTimes[*it] > 11000) {
                logMessage("Ped has been stopped for over 11 seconds, deleting entity immediately.");
                ENTITY::DELETE_ENTITY(&(*it));
                DeleteVehiclesAndHorses(vehicleEntityList, 250.0f, true); // Deletar ve�culos e cavalos
                pedStopTimes.erase(*it);
                it = entities.erase(it);
                continue;
            }
        }
        else {
            pedStopTimes.erase(*it);
        }
        ++it;
    }
    return true;
}

// Fun��o para verificar obst�culos ao redor da entidade
bool CheckEntityObstacles(Entity ped) {
    int vehicles[256];
    int totalVehicles = worldGetAllVehicles(vehicles, 256);
    Entity pedVehicle = PED::GET_VEHICLE_PED_IS_IN(ped, false);
    Entity pedMount = PED::GET_MOUNT(ped);

    // Verifica��o de obst�culos em ve�culos e montaria
    for (int i = 0; i < totalVehicles; ++i) {
        if (vehicles[i] != pedVehicle && ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(ped, vehicles[i], 1)) {
            logMessage("Obstacle detected near entity. Skipping deletion.");
            return true;
        }
    }
    return pedMount && ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(ped, pedMount, 1);
}

// Fun��o para checar intera��es entre ped e jogador
bool HandleEntityInteractions(Entity ped, Player player) {
    if (PED::IS_PED_ON_MOUNT(ped)) {
        Ped rider = PED::GET_MOUNT(ped);
        if (rider && ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(rider, player, 1) &&
            (PED::GET_IS_PED_RESPONDING_TO_POSITIVE_INTERACTION(rider, player) ||
                PED::GET_IS_PED_RESPONDING_TO_NEGATIVE_INTERACTION(rider, player))) {
            logMessage("Ped on horse is interacting with the player. Skipping deletion.");
            return true;
        }
    }

    for (int seatIndex = -1; seatIndex <= -2; ++seatIndex) {
        Ped pedInSeat = VEHICLE::GET_PED_IN_VEHICLE_SEAT(ped, seatIndex);
        if (pedInSeat && pedInSeat != player && ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(pedInSeat, player, 1) &&
            (PED::GET_IS_PED_RESPONDING_TO_POSITIVE_INTERACTION(pedInSeat, player) ||
                PED::GET_IS_PED_RESPONDING_TO_NEGATIVE_INTERACTION(pedInSeat, player))) {
            logMessage("Ped in vehicle is interacting with the player. Skipping deletion.");
            return true;
        }
    }
    return false;
}

// Fun��o para verificar o status de combate da entidade
bool HandleCombatStatus(Entity ped, Player player) {
    if (PED::IS_PED_IN_MELEE_COMBAT(ped) || PED::IS_PED_IN_COMBAT(ped, player)) {
        logMessage("Ped is in combat with player. Ignoring exclusion.");
        return true;
    }
    return false;
}

// Fun��o para excluir ve�culos e cavalos com base em condi��es espec�ficas
bool DeleteVehiclesAndHorses(std::vector<Entity>& entities, float maxDistance, bool deleteOnPlayerDeath) {
    Player playerPed = PLAYER::PLAYER_PED_ID();
    Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);

    // Checa se o jogador est� morto e deleteOnPlayerDeath est� ativado
    if (deleteOnPlayerDeath && ENTITY::IS_ENTITY_DEAD(playerPed)) {
        logMessage("Player is dead. Deleting all vehicles and horses.");
        for (Entity& entity : entities) {
            if (ENTITY::DOES_ENTITY_EXIST(entity)) ENTITY::DELETE_ENTITY(&entity);
        }
        entities.clear();
        return true;
    }

    // Verifica��o de dist�ncia e estado dos assentos do ve�culo
    for (auto it = entities.begin(); it != entities.end();) {
        if (!ENTITY::DOES_ENTITY_EXIST(*it)) {
            it = entities.erase(it);
            continue;
        }

        // Calcular a dist�ncia entre o jogador e a entidade
        float distance = BUILTIN::VDIST2(playerCoords.x, playerCoords.y, playerCoords.z,
            ENTITY::GET_ENTITY_COORDS(*it, true, false).x,
            ENTITY::GET_ENTITY_COORDS(*it, true, false).y,
            ENTITY::GET_ENTITY_COORDS(*it, true, false).z);

        // Marcar como n�o necess�rio se estiver fora do alcance
        if (distance > maxDistance * maxDistance) {
            logMessage("Vehicle or horse out of range. Marking as no longer needed.");
            ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&(*it));
            it = entities.erase(it);
            continue;
        }

        // Excluir ve�culos parados com assentos vazios
        if (VEHICLE::IS_VEHICLE_STOPPED(*it)) {
                logMessage("Vehicle stopeed. Deleting entity.");
                ENTITY::DELETE_ENTITY(&(*it));
                it = entities.erase(it);
                continue;
        }

        // Avan�ar para a pr�xima entidade caso n�o atenda �s condi��es de exclus�o
        ++it;
    }
    return true;
}
