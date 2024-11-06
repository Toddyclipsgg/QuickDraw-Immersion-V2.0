#include "header.h"

// Função que calcula o hash de uma string usando a função JOAAT (algoritmo hash específico do jogo)
Hash joaat(const char* string)
{
    return MISC::GET_HASH_KEY(string); // Usa a função nativa para obter o hash da string fornecida
}

// Função para calcular distância 3D entre duas coordenadas
float CalculateDistance(const Vector3& pos1, const Vector3& pos2) {
    return BUILTIN::VDIST(pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z);
}

// Função para calcular a distância horizontal entre duas entidades (2D)
float DistanceBetweenEntitiesHor(Entity entity1, Entity entity2) {
    if (!ENTITY::DOES_ENTITY_EXIST(entity1) || !ENTITY::DOES_ENTITY_EXIST(entity2)) {
        return 0.0f;
    }
    Vector3 pos1 = ENTITY::GET_ENTITY_COORDS(entity1, true, false);
    Vector3 pos2 = ENTITY::GET_ENTITY_COORDS(entity2, true, false);
    return MISC::GET_DISTANCE_BETWEEN_COORDS(pos1.x, pos1.y, 0.0f, pos2.x, pos2.y, 0.0f, true);
}

// Função para excluir entidades com base em condições específicas
bool DeleteEntitiesOnCondition(std::vector<Entity>& entities, float maxDistance, bool deleteOnPlayerDeath) {
    constexpr int BATCH_SIZE = 4;
    int processedEntities = 0;
    Player playerPed = PLAYER::PLAYER_PED_ID();

    if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return false;

    if (deleteOnPlayerDeath && ENTITY::IS_ENTITY_DEAD(playerPed)) {
        return DeleteEntities(entities, BATCH_SIZE);
    }

    Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);
    static std::unordered_map<Entity, int> pedStopTimes;

    for (auto it = entities.begin(); it != entities.end();) {
        if (!ENTITY::DOES_ENTITY_EXIST(*it)) {
            it = entities.erase(it);
            continue;
        }

        Vector3 entityCoords = ENTITY::GET_ENTITY_COORDS(*it, true, false);
        float distance = CalculateDistance(playerCoords, entityCoords);

        if (distance > maxDistance * maxDistance || ENTITY::IS_ENTITY_DEAD(*it)) {
            ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&(*it));
            DeleteVehiclesAndHorsesDead(vehicleEntityList, 250.0f);
            pedStopTimes.erase(*it);
            it = entities.erase(it);
            continue;
        }

        if (ShouldDeleteEntity(*it, playerPed, pedStopTimes)) {
            it = entities.erase(it);
            continue;
        }

        ++it;
        processedEntities++;

        if (processedEntities >= BATCH_SIZE) {
            break;
        }
    }
    return true;
}

bool ShouldDeleteEntity(Entity entity, Player playerPed, std::unordered_map<Entity, int>& pedStopTimes) {
    if ((PED::IS_PED_STOPPED(entity) || VEHICLE::IS_VEHICLE_STOPPED(entity)) &&
        !CheckEntityObstacles(entity) &&
        !HandleEntityInteractions(entity, playerPed) &&
        !HandleCombatStatus(entity, playerPed)) {

        if (pedStopTimes.find(entity) == pedStopTimes.end()) {
            pedStopTimes[entity] = BUILTIN::TIMERA();
        } else if (BUILTIN::TIMERA() - pedStopTimes[entity] > 11000) {
            ENTITY::DELETE_ENTITY(&entity);
            DeleteVehiclesAndHorses(vehicleEntityList, 250.0f, true);
            pedStopTimes.erase(entity);
            return true;
        }
    } else {
        pedStopTimes.erase(entity);
    }
    return false;
}

// Função para verificar obstáculos ao redor da entidade
bool CheckEntityObstacles(Entity ped) {
    if (!ENTITY::DOES_ENTITY_EXIST(ped)) return false;

    int vehicles[256];
    int totalVehicles = worldGetAllVehicles(vehicles, 256);
    Entity pedVehicle = PED::GET_VEHICLE_PED_IS_IN(ped, false);
    Entity pedMount = PED::GET_MOUNT(ped);

    for (int i = 0; i < totalVehicles; ++i) {
        if (vehicles[i] != pedVehicle && ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(ped, vehicles[i], true)) {
            return true;
        }
    }
    return pedMount && ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(ped, pedMount, true);
}

// Função para checar interações entre ped e jogador
bool HandleEntityInteractions(Entity ped, Player player) {
    if (!ENTITY::DOES_ENTITY_EXIST(ped) || !ENTITY::DOES_ENTITY_EXIST(player)) return false;

    if (PED::IS_PED_ON_MOUNT(ped)) {
        Ped rider = PED::GET_MOUNT(ped);
        if (ENTITY::DOES_ENTITY_EXIST(rider) &&
            ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(rider, player, true) &&
            (PED::GET_IS_PED_RESPONDING_TO_POSITIVE_INTERACTION(rider, player) ||
                PED::GET_IS_PED_RESPONDING_TO_NEGATIVE_INTERACTION(rider, player))) {
            return true;
        }
    }

    for (int seatIndex = -1; seatIndex >= -2; --seatIndex) {
        Ped pedInSeat = VEHICLE::GET_PED_IN_VEHICLE_SEAT(ped, seatIndex);
        if (ENTITY::DOES_ENTITY_EXIST(pedInSeat) && pedInSeat != player &&
            ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(pedInSeat, player, true) &&
            (PED::GET_IS_PED_RESPONDING_TO_POSITIVE_INTERACTION(pedInSeat, player) ||
                PED::GET_IS_PED_RESPONDING_TO_NEGATIVE_INTERACTION(pedInSeat, player))) {
            return true;
        }
    }
    return false;
}

// Função para verificar o status de combate da entidade
bool HandleCombatStatus(Entity ped, Player player) {
    if (!ENTITY::DOES_ENTITY_EXIST(ped) || !ENTITY::DOES_ENTITY_EXIST(player)) return false;

    return PED::IS_PED_IN_MELEE_COMBAT(ped) || PED::IS_PED_IN_COMBAT(ped, player);
}

// Função para excluir veículos e cavalos com base em condições específicas
bool DeleteVehiclesAndHorses(std::vector<Entity>& entities, float maxDistance, bool deleteOnPlayerDeath) {
    constexpr int BATCH_SIZE = 2;
    int processedEntities = 0;
    Player playerPed = PLAYER::PLAYER_PED_ID();

    if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return false;

    if (deleteOnPlayerDeath && ENTITY::IS_ENTITY_DEAD(playerPed)) {
        return DeleteEntities(entities, BATCH_SIZE);
    }

    for (auto it = entities.begin(); it != entities.end();) {
        if (!ENTITY::DOES_ENTITY_EXIST(*it)) {
            it = entities.erase(it);
            continue;
        }

        if (ShouldDeleteVehicleOrHorse(*it)) {
            it = entities.erase(it);
            continue;
        }

        ++it;
        processedEntities++;

        if (processedEntities >= BATCH_SIZE) {
            break;
        }
    }
    return true;
}

bool ShouldDeleteVehicleOrHorse(Entity entity) {
    if (VEHICLE::IS_VEHICLE_STOPPED(entity) || PED::_IS_MOUNT_SEAT_FREE(entity, -1) || 
        (VEHICLE::IS_VEHICLE_SEAT_FREE(entity, -1) && VEHICLE::IS_VEHICLE_SEAT_FREE(entity, -2))) {
        ENTITY::DELETE_ENTITY(&entity);
        return true;
    }
    return false;
}

// Função para excluir veículos e cavalos mortos com base em condições específicas
void DeleteVehiclesAndHorsesDead(std::vector<Entity>& entities, float maxDistance) {
    constexpr int BATCH_SIZE = 2;
    int processedEntities = 0;
    Player playerPed = PLAYER::PLAYER_PED_ID();

    if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;

    Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);

    for (auto it = entities.begin(); it != entities.end();) {
        if (!ENTITY::DOES_ENTITY_EXIST(*it)) {
            it = entities.erase(it);
            continue;
        }

        float distance = CalculateDistance(playerCoords, ENTITY::GET_ENTITY_COORDS(*it, true, false));

        if (distance > maxDistance * maxDistance || ENTITY::IS_ENTITY_DEAD(*it)) {
            ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&(*it));
            it = entities.erase(it);
            continue;
        }

        ++it;
        processedEntities++;

        if (processedEntities >= BATCH_SIZE) {
            break;
        }
    }
}

bool DeleteEntities(std::vector<Entity>& entities, int batchSize) {
    int processedEntities = 0;
    for (auto it = entities.begin(); it != entities.end();) {
        if (ENTITY::DOES_ENTITY_EXIST(*it)) {
            ENTITY::DELETE_ENTITY(&(*it));
        }
        it = entities.erase(it);
        processedEntities++;
        if (processedEntities >= batchSize) break;
    }
    return entities.empty();
}
