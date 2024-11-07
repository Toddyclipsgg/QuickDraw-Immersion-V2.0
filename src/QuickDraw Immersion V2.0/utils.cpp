#include "header.h"

// Função que calcula o hash de uma string usando a função JOAAT (algoritmo hash específico do jogo)
Hash joaat(const char* string)
{
    return MISC::GET_HASH_KEY(string); // Usa a função nativa para obter o hash da string fornecida
}

// Função para calcular distância 3D entre duas coordenadas
float CalculateDistance1(const Vector3& pos1, const Vector3& pos2) {
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

    // Verifica se o jogador existe
    if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return false;

    // Se a opção deleteOnPlayerDeath estiver ativada e o jogador estiver morto, exclui as entidades
    if (deleteOnPlayerDeath && ENTITY::IS_ENTITY_DEAD(playerPed)) {
        return DeleteEntities(entities, BATCH_SIZE);
    }

    Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);
    static std::unordered_map<Entity, int> pedStopTimes;

    // Itera sobre as entidades
    for (auto it = entities.begin(); it != entities.end();) {
        // Se a entidade não existir, remove do vetor
        if (!ENTITY::DOES_ENTITY_EXIST(*it)) {
            it = entities.erase(it);
            continue;
        }

        Vector3 entityCoords = ENTITY::GET_ENTITY_COORDS(*it, true, false);
        float distance = CalculateDistance1(playerCoords, entityCoords);

        // Se a entidade estiver fora do alcance ou morta, marca como não necessária e remove do vetor
        if (distance > maxDistance * maxDistance || ENTITY::IS_ENTITY_DEAD(*it)) {
            ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&(*it));
            DeleteVehiclesAndHorsesDead(vehicleEntityList, 250.0f);
            pedStopTimes.erase(*it);
            it = entities.erase(it);
            continue;
        }

        // Se a entidade atender às condições para exclusão, remove do vetor
        if (ShouldDeleteEntity(*it, playerPed, pedStopTimes)) {
            it = entities.erase(it);
            continue;
        }

        ++it;
        processedEntities++;

        // Limita o número de entidades processadas por iteração
        if (processedEntities >= BATCH_SIZE) {
            break;
        }
    }
    return true;
}

// Função para verificar se uma entidade deve ser excluída com base em várias condições
bool ShouldDeleteEntity(Entity entity, Player playerPed, std::unordered_map<Entity, int>& pedStopTimes) {
    // Verifica se a entidade está parada e não possui obstáculos, interações ou status de combate
    if ((PED::IS_PED_STOPPED(entity) || VEHICLE::IS_VEHICLE_STOPPED(entity)) &&
        !CheckEntityObstacles(entity) &&
        !HandleEntityInteractions(entity, playerPed) &&
        !HandleCombatStatus(entity, playerPed)) {

        // Se a entidade não estiver no mapa de tempos de parada, adiciona com o tempo atual
        if (pedStopTimes.find(entity) == pedStopTimes.end()) {
            pedStopTimes[entity] = BUILTIN::TIMERA();
        } 
        // Se a entidade estiver parada por mais de 11 segundos, exclui a entidade
        else if (BUILTIN::TIMERA() - pedStopTimes[entity] > 11000) {
            ENTITY::DELETE_ENTITY(&entity);
            DeleteVehiclesAndHorses(vehicleEntityList, 250.0f, true);
            pedStopTimes.erase(entity);
            return true;
        }
    } 
    // Se a entidade não estiver parada, remove do mapa de tempos de parada
    else {
        pedStopTimes.erase(entity);
    }
    return false;
}

// Função para verificar obstáculos ao redor da entidade
bool CheckEntityObstacles(Entity ped) {
    // Verifica se a entidade existe
    if (!ENTITY::DOES_ENTITY_EXIST(ped)) return false;

    int vehicles[256];
    // Obtém todos os veículos no mundo
    int totalVehicles = worldGetAllVehicles(vehicles, 256);
    Entity pedVehicle = PED::GET_VEHICLE_PED_IS_IN(ped, false);
    Entity pedMount = PED::GET_MOUNT(ped);

    // Itera sobre todos os veículos para verificar se há obstáculos na frente da entidade
    for (int i = 0; i < totalVehicles; ++i) {
        if (vehicles[i] != pedVehicle && ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(ped, vehicles[i], true)) {
            return true;
        }
    }
    // Verifica se há obstáculos na frente da montaria da entidade
    return pedMount && ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(ped, pedMount, true);
}

// Função para checar interações entre ped e jogador
bool HandleEntityInteractions(Entity ped, Player player) {
    // Verifica se a entidade ped ou o jogador existem
    if (!ENTITY::DOES_ENTITY_EXIST(ped) || !ENTITY::DOES_ENTITY_EXIST(player)) return false;

    // Verifica se o ped está montado em um cavalo
    if (PED::IS_PED_ON_MOUNT(ped)) {
        Ped rider = PED::GET_MOUNT(ped);
        // Verifica se o cavalo existe e tem linha de visão clara para o jogador
        if (ENTITY::DOES_ENTITY_EXIST(rider) &&
            ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(rider, player, true) &&
            // Verifica se o cavalo está respondendo a interações positivas ou negativas do jogador
            (PED::GET_IS_PED_RESPONDING_TO_POSITIVE_INTERACTION(rider, player) ||
                PED::GET_IS_PED_RESPONDING_TO_NEGATIVE_INTERACTION(rider, player))) {
            return true;
        }
    }

    // Verifica as interações para os assentos do veículo
    for (int seatIndex = -1; seatIndex >= -2; --seatIndex) {
        Ped pedInSeat = VEHICLE::GET_PED_IN_VEHICLE_SEAT(ped, seatIndex);
        // Verifica se o ped no assento existe, não é o jogador e tem linha de visão clara para o jogador
        if (ENTITY::DOES_ENTITY_EXIST(pedInSeat) && pedInSeat != player &&
            ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(pedInSeat, player, true) &&
            // Verifica se o ped no assento está respondendo a interações positivas ou negativas do jogador
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
    // Define o tamanho do lote para processamento
    constexpr int BATCH_SIZE = 2;
    int processedEntities = 0;
    Player playerPed = PLAYER::PLAYER_PED_ID();

    // Verifica se o jogador existe
    if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return false;

    // Se a opção deleteOnPlayerDeath estiver ativada e o jogador estiver morto, exclui as entidades
    if (deleteOnPlayerDeath && ENTITY::IS_ENTITY_DEAD(playerPed)) {
        return DeleteEntities(entities, BATCH_SIZE);
    }

    // Itera sobre as entidades
    for (auto it = entities.begin(); it != entities.end();) {
        // Se a entidade não existir, remove do vetor
        if (!ENTITY::DOES_ENTITY_EXIST(*it)) {
            it = entities.erase(it);
            continue;
        }

        // Se a entidade atender às condições para exclusão, remove do vetor
        if (ShouldDeleteVehicleOrHorse(*it)) {
            it = entities.erase(it);
            continue;
        }

        ++it;
        processedEntities++;

        // Limita o número de entidades processadas por iteração
        if (processedEntities >= BATCH_SIZE) {
            break;
        }
    }
    return true;
}

// Função para verificar se um veículo ou cavalo deve ser excluído com base em condições específicas
bool ShouldDeleteVehicleOrHorse(Entity entity) {
    // Verifica se o veículo está parado ou se o assento da montaria está livre
    if (VEHICLE::IS_VEHICLE_STOPPED(entity) || PED::_IS_MOUNT_SEAT_FREE(entity, -1) || 
        // Verifica se os assentos do veículo estão livres
        (VEHICLE::IS_VEHICLE_SEAT_FREE(entity, -1) && VEHICLE::IS_VEHICLE_SEAT_FREE(entity, -2))) {
        // Exclui a entidade
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

    // Verifica se o jogador existe
    if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;

    Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);

    // Itera sobre as entidades
    for (auto it = entities.begin(); it != entities.end();) {
        // Se a entidade não existir, remove do vetor
        if (!ENTITY::DOES_ENTITY_EXIST(*it)) {
            it = entities.erase(it);
            continue;
        }

        float distance1 = CalculateDistance1(playerCoords, ENTITY::GET_ENTITY_COORDS(*it, true, false));

        // Se a entidade estiver fora do alcance ou morta, marca como não necessária e remove do vetor
        if (distance1 > maxDistance * maxDistance || ENTITY::IS_ENTITY_DEAD(*it)) {
            ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&(*it));
            it = entities.erase(it);
            continue;
        }

        ++it;
        processedEntities++;

        // Limita o número de entidades processadas por iteração
        if (processedEntities >= BATCH_SIZE) {
            break;
        }
    }
}

// Função para excluir entidades em lotes
bool DeleteEntities(std::vector<Entity>& entities, int batchSize) {
    int processedEntities = 0;
    for (auto it = entities.begin(); it != entities.end();) {
        // Verifica se a entidade existe
        if (ENTITY::DOES_ENTITY_EXIST(*it)) {
            // Exclui a entidade
            ENTITY::DELETE_ENTITY(&(*it));
        }
        // Remove a entidade do vetor
        it = entities.erase(it);
        processedEntities++;
        // Limita o número de entidades processadas por iteração
        if (processedEntities >= batchSize) break;
    }
    // Retorna verdadeiro se todas as entidades foram excluídas
    return entities.empty();
}
