#include "..\header.h"

std::vector<std::string> availableOdriscollList = odriscoll;  // Lista de Peds disponíveis para gerar
std::vector<std::string> usedOdriscollList;  // Lista de Peds já utilizados
std::vector<Entity> globalEntityList;

// Função para gerar um Ped aleatório da lista e remover o modelo após o uso
std::string GetRandomOdriscollHash() {

    // Semente para o gerador de números aleatórios
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Verificar se ainda há Peds disponíveis na lista
    if (availableOdriscollList.empty()) {
        logMessage("All O'Driscolls used, resetting the list.");
        // Reiniciar a lista se todos os Peds já foram gerados
        availableOdriscollList = odriscoll;
        usedOdriscollList.clear();  // Limpa os Peds usados
    }

    // Obter um índice aleatório
    int randomIndex = std::rand() % availableOdriscollList.size();

    // Obter o hash correspondente ao índice aleatório
    std::string selectedOdriscollHash = availableOdriscollList[randomIndex];

    logMessage("O'Driscoll selected: " + selectedOdriscollHash);

    // Remover o modelo da memória para evitar duplicação
    Hash pedModel = MISC::GET_HASH_KEY(selectedOdriscollHash.c_str());
    STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(pedModel);

    // Mover o Ped selecionado para a lista de usados
    usedOdriscollList.push_back(selectedOdriscollHash);

    // Remover o Ped selecionado da lista para não gerar o mesmo
    availableOdriscollList.erase(availableOdriscollList.begin() + randomIndex);

    // Retornar o hash do Ped
    return selectedOdriscollHash;
}

// Função para mover dois peds montados para uma coordenada específica
void OdriscollsMountSpawn() {

    // ID do jogador
    Player player = PLAYER::PLAYER_ID();
    Ped playerPed = PLAYER::PLAYER_PED_ID();

    // check if player ped exists and control is on (e.g. not in a cutscene)
    if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !PLAYER::IS_PLAYER_CONTROL_ON(player)) {
        return;
    }

    // Pega as coordenadas atuais do jogador
    Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);
    logMessage("Player coordinates captured.");

    // Gerar hash para os cavalos aleatórios
    std::string randomHorse1 = GetRandomHorseHash();
    Hash horseHash1 = MISC::GET_HASH_KEY(randomHorse1.c_str());

    std::string randomHorse2 = GetRandomHorseHash();
    Hash horseHash2 = MISC::GET_HASH_KEY(randomHorse2.c_str());

    // Gerar hash para os O'Driscoll aleatórios
    std::string randomOdriscoll1 = GetRandomOdriscollHash();
    Hash odriscollHash1 = MISC::GET_HASH_KEY(randomOdriscoll1.c_str());

    std::string randomOdriscoll2 = GetRandomOdriscollHash();
    Hash odriscollHash2 = MISC::GET_HASH_KEY(randomOdriscoll2.c_str());

    // Criando os cavalos aleatórios
    Ped horse1 = createPed(horseHash1, getRandomPedPositionInRange(playerPos, 80));
    Ped horse2 = createPed(horseHash2, getRandomPedPositionInRange(playerPos, 80));
    logMessage("Horses created.");

    // Atribuir sela aos cavalos
    giveComboSaddleToHorse(horse1);
    giveComboSaddleToHorse(horse2);

    // Criar os peds montados em seus respectivos cavalos
    Ped ped1 = pedMount(odriscollHash1, horse1, true, playerPos, 80);
    Ped ped2 = pedMount(odriscollHash2, horse2, true, playerPos, 80);
    logMessage("Peds mounted on horses.");
    WAIT(4500);

    // Definir a personalidade agressiva e permitir interação
    odriscollGroupLeader(ped1);
    odriscollGroupMember(ped2);
    personalityPed(ped1);
    personalityPed(ped2);
    ConfigurePedInteraction(ped1);
    ConfigurePedInteraction(ped2);
    logMessage("Peds personality set and interactions configured.");
    WAIT(1000);

    // Verifica se ambos os peds (ped e cavalo) existem e estão vivos
    if (ENTITY::DOES_ENTITY_EXIST(ped1) && !ENTITY::IS_ENTITY_DEAD(ped1) &&
        ENTITY::DOES_ENTITY_EXIST(ped2) && !ENTITY::IS_ENTITY_DEAD(ped2)) {

        Vector3 ped1Coords = ENTITY::GET_ENTITY_COORDS(ped1, true, false);  // Pega as coordenadas atuais do ped1
        logMessage("Ped 1 coordinates captured.");
        float distanciaMaxima = 1500.0f;  // Defina a distância máxima "perto o suficiente"
        float distanciaMinima = 500.0f;   // Defina a distância mínima "longe o suficiente"
        float distancia = 0.0f;
        int tentativasMax = 25;  // Limite de tentativas para evitar loop infinito
        int tentativaAtual = 0;

        std::pair<std::string, Vector3> destino;

        // Gerar novas coordenadas até encontrar uma suficientemente perto e longe o suficiente
        while (tentativaAtual < tentativasMax) {
            destino = GetRandomCoord();  // Gerar uma coordenada aleatória
            distancia = CalcularDistancia(ped1Coords, destino.second);  // Calcular a distância a partir do ped1

            logMessage("Attempt " + std::to_string(tentativaAtual) + ": Checking distance for generated coordinate.");

            // Verifica se a distância está entre 500 e 1500 metros
            if (distancia >= distanciaMinima && distancia <= distanciaMaxima) {
                logMessage("Coordinate found within valid range.");
                break;  // Coordenada está dentro do critério de distância
            }

            tentativaAtual++;
        }

        // Se encontrou uma coordenada dentro do critério
        if (distancia >= distanciaMinima && distancia <= distanciaMaxima) {
            float x = destino.second.x;
            float y = destino.second.y;
            float z = destino.second.z;

            // Verifica se o ponto é navegável
            if (PATHFIND::IS_POINT_ON_ROAD(x, y, z, ped1)) {
                logMessage("Point is navigable, moving Ped 1.");

                // Mover o cavalo diretamente para a coordenada de destino
                TASK::TASK_MOVE_FOLLOW_ROAD_USING_NAVMESH(ped1, 1.001f, x, y, z, 0);
                ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(ped1, true);
                ManagePedBlip(ped1, player);

                // Configurar offset e garantir que ped2 siga sem ultrapassar
                float safeDistance = 2.0f;  // Distância segura atrás de ped1
                float minDistance = 1.5f;   // Distância mínima para seguir

                Vector3 ped1Position = ENTITY::GET_ENTITY_COORDS(ped1, true, false);
                Vector3 ped2Position = ENTITY::GET_ENTITY_COORDS(ped2, true, false);

                // Calcula a distância entre ped1 e ped2
                float distance = CalcularDistancia(ped1Position, ped2Position);

                logMessage("Ped 1 and Ped 2 distance calculated.");

                // Certifique-se de que ped2 sempre siga atrás de ped1, mantendo uma distância segura
                if (distance > minDistance) {
                    logMessage("Ped 2 following Ped 1.");

                    if (!ENTITY::IS_ENTITY_DEAD(ped2)) {
                        TASK::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(ped2, ped1, 0.0f, -safeDistance, 0.0f, 1.001f, -1, 0.500f, true, false, false, false, false, false);
                        ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(ped2, true);
                    }
                }
                else {
                    // Se ped2 estiver muito perto, desacelere para evitar bloqueio
                    logMessage("Ped 2 is too close to Ped 1, standing still to adjust.");
                    TASK::TASK_STAND_STILL(ped2, 500); // Espera 500ms para manter distância
                }
                ManagePedBlip(ped1, player);
            }
            else {
                // Se o ponto não for navegável, exclui os pedestres e cavalos e reinicia o script
                logMessage("Point is not navigable, deleting entities and restarting.");

                // Excluir os pedestres ped1 e ped2
                if (ENTITY::DOES_ENTITY_EXIST(ped1)) {
                    ENTITY::DELETE_ENTITY(&ped1);
                }
                if (ENTITY::DOES_ENTITY_EXIST(ped2)) {
                    ENTITY::DELETE_ENTITY(&ped2);
                }

                // Excluir os cavalos horse1 e horse2
                if (ENTITY::DOES_ENTITY_EXIST(horse1)) {
                    ENTITY::DELETE_ENTITY(&horse1);
                }
                if (ENTITY::DOES_ENTITY_EXIST(horse2)) {
                    ENTITY::DELETE_ENTITY(&horse2);
                }
                // Reiniciar o script
                OdriscollsMountSpawn();
            }
        }
    }

    // Adicionando entidades à lista global
    globalEntityList.push_back(ped1);
    globalEntityList.push_back(ped2);
    globalEntityList.push_back(horse1);
    globalEntityList.push_back(horse2);
}
