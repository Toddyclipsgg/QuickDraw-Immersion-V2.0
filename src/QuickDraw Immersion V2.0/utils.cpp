#include "header.h"

// Função para calcular distância 3D entre o ped e as coordenadas fornecidas
float CalcularDistancia(Vector3 pedCoords, Vector3 destinoCoords) {
    return BUILTIN::VDIST(pedCoords.x, pedCoords.y, pedCoords.z, destinoCoords.x, destinoCoords.y, destinoCoords.z);
}

// Função para calcular a distância entre dois pontos no espaço 3D
float calcularDistanciaObstacle(Vector3 pos1, Vector3 pos2) {
    return BUILTIN::SQRT(BUILTIN::POW(pos2.x - pos1.x, 2) + BUILTIN::POW(pos2.y - pos1.y, 2) + BUILTIN::POW(pos2.z - pos1.z, 2));
}

// Função para calcular a distância horizontal entre duas entidades (2D)
float distanceBetweenEntitiesHor(Entity entity1, Entity entity2)
{
    // Obtendo as coordenadas das entidades
    Vector3 pos1 = ENTITY::GET_ENTITY_COORDS(entity1, 1, 0);
    Vector3 pos2 = ENTITY::GET_ENTITY_COORDS(entity2, 1, 0);
    // Calculando a distância horizontal entre as entidades
    float distance = MISC::GET_DISTANCE_BETWEEN_COORDS(pos1.x, pos1.y, 0, pos2.x, pos2.y, 0, 1);
    logMessage("2D distance calculated successfully.");
    return distance;
}

// Exclui o ped se estiver parado por mais de 5 segundos, com múltiplas verificações
bool DeleteEntitiesOnCondition(std::vector<Entity>& entities, float maxDistance, bool deleteOnPlayerDeath) {
    Player playerPed = PLAYER::PLAYER_PED_ID();
    Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);

    // Deleta todas as entidades se o jogador estiver morto e deleteOnPlayerDeath for true
    if (deleteOnPlayerDeath && ENTITY::IS_ENTITY_DEAD(playerPed)) {
        logMessage("Player is dead. Deleting all entities.");
        for (Entity& entity : entities) {
            if (ENTITY::DOES_ENTITY_EXIST(entity)) ENTITY::DELETE_ENTITY(&entity);
        }
        entities.clear();
        return true;
    }

    // Mapeia o tempo de parada de cada ped
    static std::unordered_map<Entity, int> pedStopTimes;

    // Itera sobre a lista de entidades
    for (auto it = entities.begin(); it != entities.end();) {
        if (ENTITY::DOES_ENTITY_EXIST(*it)) {
            Vector3 entityCoords = ENTITY::GET_ENTITY_COORDS(*it, true, false);
            float distance = BUILTIN::VDIST(playerCoords.x, playerCoords.y, playerCoords.z, entityCoords.x, entityCoords.y, entityCoords.z);

            // 1. Verificação de proximidade do jogador e exclusão condicional com base na distância
            if (distance > maxDistance) {
                logMessage("Entity is out of range. Marking as no longer needed.");
                ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&(*it));
                pedStopTimes.erase(*it);
                it = entities.erase(it);
                continue;
            }

            // 2. Verificação de se está parado e iniciou o temporizador
            if (PED::IS_PED_STOPPED(*it)) {
                // 2a. Verifica se o ped está montado em um cavalo ou em um veículo
                if (PED::IS_PED_ON_MOUNT(*it)) {
                    Ped mount = PED::GET_MOUNT(*it);
                    if (mount && !VEHICLE::IS_VEHICLE_STOPPED(mount)) {
                        logMessage("Ped is on a moving mount, skipping deletion.");
                        ++it;
                        continue;
                    }
                }
                else if (PED::IS_PED_IN_ANY_VEHICLE(*it, false)) {
                    Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(*it, false);
                    if (veh && !VEHICLE::IS_VEHICLE_STOPPED(veh)) {
                        logMessage("Ped is in a moving vehicle, skipping deletion.");
                        ++it;
                        continue;
                    }
                }

                // 2b. Verifica interação com o jogador
                if (PED::IS_PED_IN_COMBAT(*it, playerPed) || PED::IS_PED_PERFORMING_MELEE_ACTION(*it, 8, 0) || PED::GET_IS_PED_RESPONDING_TO_POSITIVE_INTERACTION(*it, playerPed) || PED::GET_IS_PED_RESPONDING_TO_NEGATIVE_INTERACTION(*it, playerPed)) {
                    logMessage("Ped is engaged in combat with the player. Skipping deletion.");
                    ++it;
                    continue;
                }

                // 2c. Temporizador de parada
                if (pedStopTimes.find(*it) == pedStopTimes.end()) {
                    pedStopTimes[*it] = BUILTIN::TIMERA();
                    logMessage("Ped stopped, starting timer.");
                }
                else {
                    int stoppedDuration = BUILTIN::TIMERA() - pedStopTimes[*it];
                    if (stoppedDuration > 5000) {
                        logMessage("Ped stopped for over 5 seconds, deleting entity.");
                        ENTITY::DELETE_ENTITY(&(*it));
                        pedStopTimes.erase(*it);
                        it = entities.erase(it);
                        continue;
                    }
                }
            }
            else {
                if (pedStopTimes.find(*it) != pedStopTimes.end()) {
                    logMessage("Ped started moving again, resetting timer.");
                    pedStopTimes.erase(*it);
                }
            }
        }
        else {
            ++it;
        }
    }

    WAIT(500); // Intervalo para economizar processamento
    return true;
}

// Exclui veículos e cavalos parados com verificações adicionais antes de marcar como desnecessários
bool DeleteVehiclesAndHorses(std::vector<Entity>& entities, float maxDistance, bool deleteOnPlayerDeath) {
    Player playerPed = PLAYER::PLAYER_PED_ID();
    Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);

    // Se o jogador está morto e deleteOnPlayerDeath está ativado
    if (deleteOnPlayerDeath && ENTITY::IS_ENTITY_DEAD(playerPed)) {
        logMessage("Player is dead. Deleting all vehicles and horses.");
        for (Entity& entity : entities) {
            if (ENTITY::DOES_ENTITY_EXIST(entity)) ENTITY::DELETE_ENTITY(&entity);
        }
        entities.clear();
        return true;
    }

    static std::unordered_map<Entity, int> entityStopTimes;

    for (auto it = entities.begin(); it != entities.end();) {
        if (ENTITY::DOES_ENTITY_EXIST(*it)) {
            Vector3 entityCoords = ENTITY::GET_ENTITY_COORDS(*it, true, false);
            float distance = BUILTIN::VDIST(playerCoords.x, playerCoords.y, playerCoords.z, entityCoords.x, entityCoords.y, entityCoords.z);

            // 1. Verificação de distância e estado de parada da entidade
            if (distance > maxDistance) {
                logMessage("Entity out of range. Marking as no longer needed.");
                ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&(*it));
                entityStopTimes.erase(*it);
                it = entities.erase(it);
                continue;
            }

            // 2. Verificação se veículo ou cavalo está parado
            if (VEHICLE::IS_VEHICLE_STOPPED(*it) || (PED::IS_PED_ON_MOUNT(*it) && PED::IS_PED_STOPPED(PED::GET_MOUNT(*it)))) {
                // 2a. Verifica se o veículo/cavalo está próximo de outros peds/veículos (detecção de obstáculos)
                int nearbyEntities[256];
                int nearbyPedCount = worldGetAllPeds(nearbyEntities, 256);
                int nearbyVehicleCount = worldGetAllVehicles(nearbyEntities, 256);
                bool obstacleDetected = false;

                for (int i = 0; i < nearbyPedCount && !obstacleDetected; ++i) {
                    Vector3 nearbyPedCoords = ENTITY::GET_ENTITY_COORDS(nearbyEntities[i], true, false);
                    if (BUILTIN::VDIST(entityCoords.x, entityCoords.y, entityCoords.z, nearbyPedCoords.x, nearbyPedCoords.y, nearbyPedCoords.z) < 2.0f) {
                        obstacleDetected = true;
                    }
                }

                for (int i = 0; i < nearbyVehicleCount && !obstacleDetected; ++i) {
                    Vector3 nearbyVehicleCoords = ENTITY::GET_ENTITY_COORDS(nearbyEntities[i], true, false);
                    if (BUILTIN::VDIST(entityCoords.x, entityCoords.y, entityCoords.z, nearbyVehicleCoords.x, nearbyVehicleCoords.y, nearbyVehicleCoords.z) < 2.0f) {
                        obstacleDetected = true;
                    }
                }

                if (obstacleDetected) {
                    logMessage("Obstacle detected near entity, skipping deletion.");
                    ++it;
                    continue;
                }

                // 2b. Temporizador para entidades paradas
                if (entityStopTimes.find(*it) == entityStopTimes.end()) {
                    entityStopTimes[*it] = BUILTIN::TIMERA();
                    logMessage("Vehicle or horse stopped, starting timer.");
                }
                else {
                    int stoppedDuration = BUILTIN::TIMERA() - entityStopTimes[*it];
                    if (stoppedDuration > 5000) {
                        logMessage("Vehicle or horse stopped for over 5 seconds, deleting entity.");
                        ENTITY::DELETE_ENTITY(&(*it));
                        entityStopTimes.erase(*it);
                        it = entities.erase(it);
                        continue;
                    }
                }
            }
            else {
                if (entityStopTimes.find(*it) != entityStopTimes.end()) {
                    logMessage("Vehicle or horse started moving again, resetting timer.");
                    entityStopTimes.erase(*it);
                }
            }
        }
        else {
            ++it;
        }
    }

    WAIT(500);
    return true;
}

/*
// Exclua o ped se ele for parado por mais de 5 segundos ou defina a entidade como desnecessária caso contrário
bool DeleteEntitiesOnCondition(std::vector<Entity>& entities, float maxDistance, bool deleteOnPlayerDeath) {

    // Definir ped do jogador
    Player playerPed = PLAYER::PLAYER_PED_ID();

    // Checar se o jogador está morto e, se deleteOnPlayerDeath estiver ativado, deletar todas as entidades
    if (deleteOnPlayerDeath && ENTITY::IS_ENTITY_DEAD(playerPed)) {
        logMessage("Player is dead. Deleting all entities.");
        for (Entity& entity : entities) {
            if (ENTITY::DOES_ENTITY_EXIST(entity)) {
                ENTITY::DELETE_ENTITY(&entity);
            }
        }
        entities.clear();  // Limpar a lista de entidades
        return true;
    }

    // Pegar coordenadas do jogador
    Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);

    // Variável para armazenar o tempo em que o ped parou
    static std::unordered_map<Entity, int> pedStopTimes;

    // Iterar sobre a lista de entidades
    for (auto it = entities.begin(); it != entities.end(); ) {
        if (ENTITY::DOES_ENTITY_EXIST(*it)) {

            // Pegar coordenadas da entidade
            Vector3 entityCoords = ENTITY::GET_ENTITY_COORDS(*it, true, false);

            // Calcular a distância entre o jogador e a entidade usando o BUILTIN::VDIST
            float distance = BUILTIN::VDIST(playerCoords.x, playerCoords.y, playerCoords.z,
                entityCoords.x, entityCoords.y, entityCoords.z);

            // Se a entidade estiver parada
            if (PED::IS_PED_STOPPED(*it)) {

                // ID do jogador
                Player player = PLAYER::PLAYER_ID();

                // Verificar se algum pedestre montado no cavalo está interagindo com o jogador
                Ped rider = PED::IS_PED_ON_MOUNT(*it);  // Obtenha o ped montado no cavalo
                if (rider != 0 && rider) {
                    if (PED::GET_IS_PED_RESPONDING_TO_POSITIVE_INTERACTION(rider, player) || PED::GET_IS_PED_RESPONDING_TO_NEGATIVE_INTERACTION(rider, player)) {
                        logMessage("Ped on mount is interacting with the player. Cancelling deletion.");
                        return false;  // Cancelar a exclusão do cavalo se o ped estiver interagindo com o jogador
                    }
                }

                // Verificar se algum pedestre no veículo está interagindo com o jogador
                for (int seatIndex = -1; seatIndex <= 0; ++seatIndex) {  // Assentos: motorista e passageiro
                    Ped pedInSeat = VEHICLE::GET_PED_IN_VEHICLE_SEAT(*it, seatIndex);
                    if (pedInSeat != 0 && pedInSeat != PLAYER::PLAYER_PED_ID()) {
                        if (PED::GET_IS_PED_RESPONDING_TO_POSITIVE_INTERACTION(pedInSeat, player) || PED::GET_IS_PED_RESPONDING_TO_NEGATIVE_INTERACTION(pedInSeat, player)) {
                            logMessage("Ped in vehicle is interacting with the player. Cancelling deletion.");
                            return false;  // Cancelar a exclusão do veículo se o ped estiver interagindo com o jogador
                        }
                    }
                }

                int peds[256];  // Array para armazenar peds encontrados
                int totalPeds = worldGetAllPeds(peds, 256);

                // Posição do ped
                Vector3 pedVehicle = ENTITY::GET_ENTITY_COORDS(*it, true, false);

                for (int i = 0; i < totalPeds; ++i) {
                    Ped pedWorld2 = peds[i];

                    Vector3 pedWorld = ENTITY::GET_ENTITY_COORDS(pedWorld2, true, false);
                    float distancia = calcularDistanciaObstacle(pedVehicle, pedWorld);

                    if (distancia <= 10.0f) return false;
                }

                if (pedStopTimes.find(*it) == pedStopTimes.end()) {
                    // Se o ped acabou de parar, registrar o tempo atual
                    pedStopTimes[*it] = BUILTIN::TIMERA();
                    logMessage("Ped stopped, starting timer.");
                }
                else {
                    // Verificar se o ped está parado por mais de 5 segundos
                    int stoppedDuration = BUILTIN::TIMERA() - pedStopTimes[*it];
                    if (stoppedDuration > 5000) {  // Mais de 5 segundos (5000 ms)
                        logMessage("Ped has been stopped for over 10 seconds, deleting entity immediately.");
                        ENTITY::DELETE_ENTITY(&(*it));  // Delete the entity immediately
                        it = entities.erase(it);  // Remover entidade da lista
                        pedStopTimes.erase(*it);  // Remover ped da lista de tempos
                        continue;  // Ir para a próxima entidade
                    }
                }
            }
            else {
                // Se o ped começou a se mover novamente, remover do mapa de pedStopTimes
                if (pedStopTimes.find(*it) != pedStopTimes.end()) {
                    logMessage("Ped started moving again, resetting timer.");
                    pedStopTimes.erase(*it);
                }
            }
            // Se a distância for maior que maxDistance ou a entidade estiver morta, marcar a entidade como não mais necessária
            if (distance > maxDistance || ENTITY::IS_ENTITY_DEAD(*it)) {
                logMessage("Marking entity as no longer needed: out of range or dead.");
                ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&(*it));  // Set the entity as no longer needed
                it = entities.erase(it);  // Remover entidade da lista
                pedStopTimes.erase(*it);
            }
            else {
                ++it;  // Continuar para a próxima entidade
            }
        }
        else {
            ++it;  // Continuar para a próxima entidade
        }
    }
    // Adicionar um momento de pausa para evitar sobrecarregar o processamento
    WAIT(2000);
    return true;
}
*/
