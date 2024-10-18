#include "..\header.h"

std::vector<std::string> availableSkinnerList = skinner;  // Lista de Peds dispon�veis para gerar
std::vector<std::string> usedSkinnerList;  // Lista de Peds j� utilizados


void skinnerGroup(Ped ped1, Ped ped2) {
    int skinnerId = PED::CREATE_GROUP(0);

    if (ENTITY::DOES_ENTITY_EXIST(ped1) && !ENTITY::IS_ENTITY_DEAD(ped1)) {

        PED::SET_PED_AS_GROUP_LEADER(ped1, skinnerId, false);
        PED::SET_GROUP_FORMATION(skinnerId, 12);
        PED::SET_GROUP_FORMATION_SPACING(skinnerId, 2.0f, 1.0f, 2.0f);
        DECORATOR::DECOR_SET_INT(ped1, "skinner", 0);
    }

    if (ENTITY::DOES_ENTITY_EXIST(ped2) && !ENTITY::IS_ENTITY_DEAD(ped2)) {

        PED::SET_PED_AS_GROUP_MEMBER(ped2, skinnerId);
        PED::SET_GROUP_FORMATION(skinnerId, 12);
        PED::SET_GROUP_FORMATION_SPACING(skinnerId, 2.0f, 1.0f, 2.0f);
        DECORATOR::DECOR_SET_INT(ped2, "skinner", 0);
    }

    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(4, REL_GANG_SKINNER_BROTHERS, RELGROUPHASH_PLAYER);
}

// Fun��o para gerar um Ped aleat�rio da lista e remover o modelo ap�s o uso
std::string GetRandomSkinnerHash() {

    // Semente para o gerador de n�meros aleat�rios
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Verificar se ainda h� Peds dispon�veis na lista
    if (availableSkinnerList.empty()) {
        logMessage("All O'Driscolls used, resetting the list.");
        // Reiniciar a lista se todos os Peds j� foram gerados
        availableSkinnerList = skinner;
        usedSkinnerList.clear();  // Limpa os Peds usados
    }

    // Obter um �ndice aleat�rio
    int randomIndex = std::rand() % availableSkinnerList.size();

    // Obter o hash correspondente ao �ndice aleat�rio
    std::string selectedSkinnerHash = availableSkinnerList[randomIndex];

    logMessage("O'Driscoll selected: " + selectedSkinnerHash);

    // Remover o modelo da mem�ria para evitar duplica��o
    Hash pedModel = MISC::GET_HASH_KEY(selectedSkinnerHash.c_str());
    STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(pedModel);

    // Mover o Ped selecionado para a lista de usados
    usedSkinnerList.push_back(selectedSkinnerHash);

    // Remover o Ped selecionado da lista para n�o gerar o mesmo
    availableSkinnerList.erase(availableSkinnerList.begin() + randomIndex);

    // Retornar o hash do Ped
    return selectedSkinnerHash;
}

// Fun��o para mover dois peds montados para uma coordenada espec�fica
void SkinnerMountSpawn() {
    // Desativa durante miss�o
    if (MISC::GET_MISSION_FLAG()) {
        return;
    }

    // ID do jogador
    Player player = PLAYER::PLAYER_ID();
    Ped playerPed = PLAYER::PLAYER_PED_ID();

    // Pega as coordenadas atuais do jogador
    Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);
    logMessage("Player coordinates captured.");
    // Gerar hash para os cavalos aleat�rios
    std::string randomHorse1 = GetRandomHorseHash();
    Hash horseHash1 = MISC::GET_HASH_KEY(randomHorse1.c_str());
    std::string randomHorse2 = GetRandomHorseHash();
    Hash horseHash2 = MISC::GET_HASH_KEY(randomHorse2.c_str());
    // Gerar hash para os Skinner aleat�rios
    std::string randomSkinner1 = GetRandomSkinnerHash();
    Hash skinnerHash1 = MISC::GET_HASH_KEY(randomSkinner1.c_str());
    std::string randomSkinner2 = GetRandomSkinnerHash();
    Hash skinnerHash2 = MISC::GET_HASH_KEY(randomSkinner2.c_str());
    // Criando os cavalos aleat�rios
    Ped horse1 = createPed(horseHash1, getRandomPedPositionInRange(playerPos, 80));
    Ped horse2 = createPed(horseHash2, getRandomPedPositionInRange(playerPos, 80));
    logMessage("Horses created.");
    // Atribuir sela aos cavalos
    giveComboSaddleToHorse(horse1);
    giveComboSaddleToHorse(horse2);
    // Criar os peds montados em seus respectivos cavalos
    Ped ped1 = pedMount(skinnerHash1, horse1, true, playerPos, 80);
    Ped ped2 = pedMount(skinnerHash2, horse2, true, playerPos, 80);
    logMessage("Peds mounted on horses.");
    WAIT(4500);
    // Definir a personalidade agressiva e permitir intera��o
    skinnerGroup(ped1, ped2);
    personalityPed(ped1);
    personalityPed(ped2);
    ConfigurePedInteraction(ped1);
    ConfigurePedInteraction(ped2);
    logMessage("Peds personality set and interactions configured.");
    WAIT(1000);
    // Verifica se ambos os peds (ped e cavalo) existem e est�o vivos
    if (ENTITY::DOES_ENTITY_EXIST(ped1) && !ENTITY::IS_ENTITY_DEAD(ped1) &&
        ENTITY::DOES_ENTITY_EXIST(ped2) && !ENTITY::IS_ENTITY_DEAD(ped2)) {
        Vector3 ped1Coords = ENTITY::GET_ENTITY_COORDS(ped1, true, false);  // Pega as coordenadas atuais do ped1
        logMessage("Ped 1 coordinates captured.");
        float distanciaMaxima = 1500.0f;  // Defina a dist�ncia m�xima "perto o suficiente"
        float distanciaMinima = 500.0f;   // Defina a dist�ncia m�nima "longe o suficiente"
        float distancia = 0.0f;
        int tentativasMax = 25;  // Limite de tentativas para evitar loop infinito
        int tentativaAtual = 0;
        std::pair<std::string, Vector3> destino;
        // Gerar novas coordenadas at� encontrar uma suficientemente perto e longe o suficiente
        while (tentativaAtual < tentativasMax) {
            destino = GetRandomCoord();  // Gerar uma coordenada aleat�ria
            distancia = CalcularDistancia(ped1Coords, destino.second);  // Calcular a dist�ncia a partir do ped1
            logMessage("Attempt " + std::to_string(tentativaAtual) + ": Checking distance for generated coordinate.");
            // Verifica se a dist�ncia est� entre 500 e 1500 metros
            if (distancia >= distanciaMinima && distancia <= distanciaMaxima) {
                logMessage("Coordinate found within valid range.");
                break;  // Coordenada est� dentro do crit�rio de dist�ncia
            }
            tentativaAtual++;
        }
        // Se encontrou uma coordenada dentro do crit�rio
        if (distancia >= distanciaMinima && distancia <= distanciaMaxima) {
            float x = destino.second.x;
            float y = destino.second.y;
            float z = destino.second.z;
            // Verifica se o ponto � naveg�vel
            if (PATHFIND::IS_POINT_ON_ROAD(x, y, z, ped1)) {
                logMessage("Point is navigable, moving Ped 1.");
                // Mover o cavalo diretamente para a coordenada de destino
                TASK::TASK_MOVE_FOLLOW_ROAD_USING_NAVMESH(ped1, 1.001f, x, y, z, 0);
                ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(ped1, true);
                ManagePedBlip(ped1, player);
                // Configurar offset e garantir que ped2 siga sem ultrapassar
                float safeDistance = 2.0f;  // Dist�ncia segura atr�s de ped1
                float minDistance = 1.5f;   // Dist�ncia m�nima para seguir
                Vector3 ped1Position = ENTITY::GET_ENTITY_COORDS(ped1, true, false);
                Vector3 ped2Position = ENTITY::GET_ENTITY_COORDS(ped2, true, false);
                // Calcula a dist�ncia entre ped1 e ped2
                float distance = CalcularDistancia(ped1Position, ped2Position);
                logMessage("Ped 1 and Ped 2 distance calculated.");
                // Certifique-se de que ped2 sempre siga atr�s de ped1, mantendo uma dist�ncia segura
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
                    TASK::TASK_STAND_STILL(ped2, 500); // Espera 500ms para manter dist�ncia
                }
            }
            else {
                logMessage("NavMesh path query failed for ped1. Deleting entities and restarting.");

                ENTITY::DELETE_ENTITY(&ped1);
                ENTITY::DELETE_ENTITY(&ped2);
                ENTITY::DELETE_ENTITY(&horse1);
                ENTITY::DELETE_ENTITY(&horse2);
                SkinnerMountSpawn();
            }

            // Define o tempo necess�rio para que o ped fique parado (5 segundos em milissegundos)
            const int tempoParadoNecessario = 5000;
            // Define o tempo m�ximo para encerrar o script (6 segundos em milissegundos)
            const int tempoMaximoExecucao = 6000;

            BUILTIN::SETTIMERA(0); // Zera o temporizador no in�cio
            int tempoTotal = 0; // Vari�vel para armazenar o tempo total de execu��o

            while (true) {
                // Verifica se o ped est� parado
                if (!PED::IS_PED_STOPPED(ped1)) {
                    logMessage("SKINNER: Ped voltou a se mover.");
                    BUILTIN::SETTIMERA(0); // Reseta o temporizador se o ped se mover
                }

                // Se o ped est� parado, verificamos o tempo no temporizador
                if (BUILTIN::TIMERA() >= tempoParadoNecessario) {
                    logMessage("SKINNER: Ped est� parado h� 5 segundos. Apagando...");

                    ENTITY::DELETE_ENTITY(&ped1);
                    ENTITY::DELETE_ENTITY(&ped2);
                    ENTITY::DELETE_ENTITY(&horse1);
                    ENTITY::DELETE_ENTITY(&horse2);
                    SkinnerMountSpawn();
                    break; // Sai do loop ap�s apagar as entidades
                }

                // Verifica se o tempo total de execu��o chegou a 6 segundos
                tempoTotal += 100;
                if (tempoTotal >= tempoMaximoExecucao) {
                    break; // Sai do loop ap�s 6 segundos
                }

                // Aguarda 100 milissegundos antes de verificar novamente
                WAIT(100);
            }
        }
    }
    // Adicionando entidades � lista global
    globalEntityList.push_back(ped1);
    globalEntityList.push_back(ped2);
    globalEntityList.push_back(horse1);
    globalEntityList.push_back(horse2);
}