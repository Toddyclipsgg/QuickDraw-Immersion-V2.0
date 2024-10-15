#include "header.h"

std::vector<Entity> globalEntityList;

// Função para calcular distância 3D entre o ped e as coordenadas fornecidas
float CalcularDistancia(Vector3 pedCoords, Vector3 destinoCoords) {
    return BUILTIN::VDIST(pedCoords.x, pedCoords.y, pedCoords.z, destinoCoords.x, destinoCoords.y, destinoCoords.z);
}

// Delete ped if stopped for more than 5 seconds or set entity as no longer needed otherwise
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
                if (pedStopTimes.find(*it) == pedStopTimes.end()) {
                    // Se o ped acabou de parar, registrar o tempo atual
                    pedStopTimes[*it] = BUILTIN::TIMERA();
                    logMessage("Ped stopped, starting timer.");
                }
                else {
                    // Verificar se o ped está parado por mais de 5 segundos
                    int stoppedDuration = BUILTIN::TIMERA() - pedStopTimes[*it];
                    if (stoppedDuration > 6000) {  // Mais de 7 segundos (7000 ms)
                        logMessage("Ped has been stopped for over 7 seconds, deleting entity immediately.");
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
