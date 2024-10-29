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

// Exclua o ped se ele for parado por mais de 5 segundos ou defina a entidade como desnecess�ria caso contr�rio
bool DeleteEntitiesOnCondition(std::vector<Entity>& entities, float maxDistance, bool deleteOnPlayerDeath) {
    // Define o ped do jogador e obt�m suas coordenadas
    Player playerPed = PLAYER::PLAYER_PED_ID();
    Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);

    // Verifica se o jogador est� morto; caso positivo e deleteOnPlayerDeath ativado, exclui todas as entidades
    if (deleteOnPlayerDeath && ENTITY::IS_ENTITY_DEAD(playerPed)) {
        logMessage("Player is dead. Deleting all entities.");
        for (Entity& entity : entities) {
            if (ENTITY::DOES_ENTITY_EXIST(entity)) ENTITY::DELETE_ENTITY(&entity);
        }
        entities.clear();  // Limpa a lista ap�s a exclus�o
        return true;
    }

    // Armazena o tempo de parada para cada ped em um mapa
    static std::unordered_map<Entity, int> pedStopTimes;

    // Itera pela lista de entidades
    for (auto it = entities.begin(); it != entities.end();) {
        // Verifica se a entidade existe
        if (ENTITY::DOES_ENTITY_EXIST(*it)) {
            // Obt�m as coordenadas da entidade e calcula a dist�ncia do jogador at� ela
            Vector3 entityCoords = ENTITY::GET_ENTITY_COORDS(*it, true, false);
            float distance = BUILTIN::VDIST(playerCoords.x, playerCoords.y, playerCoords.z, entityCoords.x, entityCoords.y, entityCoords.z);

            // Verifica se o ped est� parado
            if (PED::IS_PED_STOPPED(*it)) {
                Player player = PLAYER::PLAYER_ID();

                // Verifica se o ped est� montado em um cavalo e interagindo com o jogador; se sim, pula a exclus�o
                if (PED::IS_PED_ON_MOUNT(*it)) {
                    Ped rider = PED::GET_MOUNT(*it);
                    if (rider && (PED::GET_IS_PED_RESPONDING_TO_POSITIVE_INTERACTION(rider, player) || PED::GET_IS_PED_RESPONDING_TO_NEGATIVE_INTERACTION(rider, player))) {
                        logMessage("Ped on mount is interacting with the player. Skipping deletion.");
                        ++it;
                        continue;
                    }
                }

                // Checa se o ped no ve�culo est� interagindo com o jogador
                for (int seatIndex = -1; seatIndex <= -2; ++seatIndex) {
                    Ped pedInSeat = VEHICLE::GET_PED_IN_VEHICLE_SEAT(*it, seatIndex);
                    if (pedInSeat && pedInSeat != playerPed) {
                        if (PED::GET_IS_PED_RESPONDING_TO_POSITIVE_INTERACTION(pedInSeat, player) || PED::GET_IS_PED_RESPONDING_TO_NEGATIVE_INTERACTION(pedInSeat, player)) {
                            logMessage("Ped in vehicle is interacting with the player. Skipping deletion.");
                            ++it;
                            continue;
                        }
                    }
                }

                // Verifica e inicia o temporizador se o ped estiver parado pela primeira vez
                if (pedStopTimes.find(*it) == pedStopTimes.end()) {
                    pedStopTimes[*it] = BUILTIN::TIMERA();
                    logMessage("Ped stopped, starting timer.");
                }
                else {

                    // Verifica a presen�a de obst�culos ao redor do ped
                    int peds[256];
                    int totalPeds = worldGetAllPeds(peds, 256);
                    int vehicles[256];
                    int totalVehicles = worldGetAllVehicles(vehicles, 256);
                    Vector3 pedVehiclePos = ENTITY::GET_ENTITY_COORDS(*it, true, false);
                    bool obstacleDetected = false;

                    // Verifica��o de obst�culos entre peds pr�ximos
                    for (int i = 0; i < totalPeds; ++i) {
                        if (PED::_GET_META_PED_TYPE(peds[i]) == MPT_ANIMAL && ENTITY::DOES_ENTITY_EXIST(peds[i])) {
                            continue;  // Ignora animais
                        }
                        Vector3 pedWorld = ENTITY::GET_ENTITY_COORDS(peds[i], true, false);
                        if (CalcularDistancia(pedVehiclePos, pedWorld) <= 0.0f) {
                            obstacleDetected = true;
                            break;
                        }
                    }

                    // Verifica��o de obst�culos entre ve�culos pr�ximos
                    for (int i = 0; i < totalVehicles; ++i) {
                        Vector3 vehicleWorld = ENTITY::GET_ENTITY_COORDS(vehicles[i], true, false);
                        if (CalcularDistancia(pedVehiclePos, vehicleWorld) <= 0.0f) {
                            obstacleDetected = true;
                            break;
                        }
                    }

                    // Se houver obst�culos, pula a exclus�o
                    if (obstacleDetected) {
                        logMessage("Obstacle detected near entity. Skipping deletion.");
                        ++it;
                        continue;
                    }

                    // Se n�o houver obst�culos, verifica se o ped est� parado por mais de 5 segundos
                    int stoppedDuration = BUILTIN::TIMERA() - pedStopTimes[*it];
                    if (stoppedDuration > 5000) {
                        logMessage("Ped has been stopped for over 5 seconds, deleting entity.");
                        ENTITY::DELETE_ENTITY(&(*it));
                        DeleteVehiclesAndHorses(vehicleEntityList, 250.0f, true);
                        pedStopTimes.erase(*it);
                        it = entities.erase(it);
                        continue;
                    }
                }
            }
            else {
                // Se o ped come�ar a se mover, remove-o do temporizador
                if (pedStopTimes.find(*it) != pedStopTimes.end()) {
                    logMessage("Ped started moving again, resetting timer.");
                    pedStopTimes.erase(*it);
                }
            }

            // Exclui a entidade se estiver fora de alcance ou morta
            if (distance > maxDistance || ENTITY::IS_ENTITY_DEAD(*it)) {
                logMessage("Marking entity as no longer needed: out of range or dead.");
                ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&(*it));
                DeleteVehiclesAndHorses(vehicleEntityList, 250.0f, true); // Chama a exclus�o de ve�culos
                pedStopTimes.erase(*it);
                it = entities.erase(it);
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }

    // Intervalo entre as verifica��es para evitar processamento excessivo
    WAIT(500);
    return true;
}

// Fun��o para excluir ve�culos e cavalos ap�s a fun��o principal
bool DeleteVehiclesAndHorses(std::vector<Entity>& entities, float maxDistance, bool deleteOnPlayerDeath) {
    Player playerPed = PLAYER::PLAYER_PED_ID();
    Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);

    // Se o jogador est� morto e deleteOnPlayerDeath est� ativado, exclui todas as entidades
    if (deleteOnPlayerDeath && ENTITY::IS_ENTITY_DEAD(playerPed)) {
        logMessage("Player is dead. Deleting all vehicles and horses.");
        for (Entity& entity : entities) {
            if (ENTITY::DOES_ENTITY_EXIST(entity)) ENTITY::DELETE_ENTITY(&entity);
        }
        entities.clear();
        return true;
    }

    // Temporizador para entidades paradas
    static std::unordered_map<Entity, int> entityStopTimes;

    for (auto it = entities.begin(); it != entities.end();) {
        if (ENTITY::DOES_ENTITY_EXIST(*it)) {
            Vector3 entityCoords = ENTITY::GET_ENTITY_COORDS(*it, true, false);
            float distance = BUILTIN::VDIST(playerCoords.x, playerCoords.y, playerCoords.z, entityCoords.x, entityCoords.y, entityCoords.z);

            // Verifica se a entidade est� parada
            if (VEHICLE::IS_VEHICLE_STOPPED(*it) || PED::IS_PED_STOPPED(*it)) {
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

            // Exclui a entidade se estiver fora do limite de dist�ncia ou estiver morta
            if (distance > maxDistance || ENTITY::IS_ENTITY_DEAD(*it)) {
                logMessage("Marking vehicle or horse as no longer needed: out of range or dead.");
                ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&(*it));
                entityStopTimes.erase(*it);
                it = entities.erase(it);
            }
            else {
                ++it;
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
// Exclua o ped se ele for parado por mais de 5 segundos ou defina a entidade como desnecess�ria caso contr�rio
bool DeleteEntitiesOnCondition(std::vector<Entity>& entities, float maxDistance, bool deleteOnPlayerDeath) {

    // Definir ped do jogador
    Player playerPed = PLAYER::PLAYER_PED_ID();

    // Checar se o jogador est� morto e, se deleteOnPlayerDeath estiver ativado, deletar todas as entidades
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

    // Vari�vel para armazenar o tempo em que o ped parou
    static std::unordered_map<Entity, int> pedStopTimes;

    // Iterar sobre a lista de entidades
    for (auto it = entities.begin(); it != entities.end(); ) {
        if (ENTITY::DOES_ENTITY_EXIST(*it)) {

            // Pegar coordenadas da entidade
            Vector3 entityCoords = ENTITY::GET_ENTITY_COORDS(*it, true, false);

            // Calcular a dist�ncia entre o jogador e a entidade usando o BUILTIN::VDIST
            float distance = BUILTIN::VDIST(playerCoords.x, playerCoords.y, playerCoords.z,
                entityCoords.x, entityCoords.y, entityCoords.z);

            // Se a entidade estiver parada
            if (PED::IS_PED_STOPPED(*it)) {

                // ID do jogador
                Player player = PLAYER::PLAYER_ID();

                // Verificar se algum pedestre montado no cavalo est� interagindo com o jogador
                Ped rider = PED::IS_PED_ON_MOUNT(*it);  // Obtenha o ped montado no cavalo
                if (rider != 0 && rider) {
                    if (PED::GET_IS_PED_RESPONDING_TO_POSITIVE_INTERACTION(rider, player) || PED::GET_IS_PED_RESPONDING_TO_NEGATIVE_INTERACTION(rider, player)) {
                        logMessage("Ped on mount is interacting with the player. Cancelling deletion.");
                        return false;  // Cancelar a exclus�o do cavalo se o ped estiver interagindo com o jogador
                    }
                }

                // Verificar se algum pedestre no ve�culo est� interagindo com o jogador
                for (int seatIndex = -1; seatIndex <= 0; ++seatIndex) {  // Assentos: motorista e passageiro
                    Ped pedInSeat = VEHICLE::GET_PED_IN_VEHICLE_SEAT(*it, seatIndex);
                    if (pedInSeat != 0 && pedInSeat != PLAYER::PLAYER_PED_ID()) {
                        if (PED::GET_IS_PED_RESPONDING_TO_POSITIVE_INTERACTION(pedInSeat, player) || PED::GET_IS_PED_RESPONDING_TO_NEGATIVE_INTERACTION(pedInSeat, player)) {
                            logMessage("Ped in vehicle is interacting with the player. Cancelling deletion.");
                            return false;  // Cancelar a exclus�o do ve�culo se o ped estiver interagindo com o jogador
                        }
                    }
                }

                int peds[256];  // Array para armazenar peds encontrados
                int totalPeds = worldGetAllPeds(peds, 256);

                // Posi��o do ped
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
                    // Verificar se o ped est� parado por mais de 5 segundos
                    int stoppedDuration = BUILTIN::TIMERA() - pedStopTimes[*it];
                    if (stoppedDuration > 5000) {  // Mais de 5 segundos (5000 ms)
                        logMessage("Ped has been stopped for over 10 seconds, deleting entity immediately.");
                        ENTITY::DELETE_ENTITY(&(*it));  // Delete the entity immediately
                        it = entities.erase(it);  // Remover entidade da lista
                        pedStopTimes.erase(*it);  // Remover ped da lista de tempos
                        continue;  // Ir para a pr�xima entidade
                    }
                }
            }
            else {
                // Se o ped come�ou a se mover novamente, remover do mapa de pedStopTimes
                if (pedStopTimes.find(*it) != pedStopTimes.end()) {
                    logMessage("Ped started moving again, resetting timer.");
                    pedStopTimes.erase(*it);
                }
            }
            // Se a dist�ncia for maior que maxDistance ou a entidade estiver morta, marcar a entidade como n�o mais necess�ria
            if (distance > maxDistance || ENTITY::IS_ENTITY_DEAD(*it)) {
                logMessage("Marking entity as no longer needed: out of range or dead.");
                ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&(*it));  // Set the entity as no longer needed
                it = entities.erase(it);  // Remover entidade da lista
                pedStopTimes.erase(*it);
            }
            else {
                ++it;  // Continuar para a pr�xima entidade
            }
        }
        else {
            ++it;  // Continuar para a pr�xima entidade
        }
    }
    // Adicionar um momento de pausa para evitar sobrecarregar o processamento
    WAIT(2000);
    return true;
}
*/
