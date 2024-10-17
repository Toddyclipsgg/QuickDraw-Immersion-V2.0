#include "..\header.h"

std::vector<std::string> availableLemoyneList = lemoyne;  // Lista de Peds dispon�veis para gerar
std::vector<std::string> usedLemoyneList;  // Lista de Peds j� utilizados

void lemoyneGroup(Ped ped1, Ped ped2) {
    int lemoyneId = PED::CREATE_GROUP(0);

    if (ENTITY::DOES_ENTITY_EXIST(ped1) && !ENTITY::IS_ENTITY_DEAD(ped1)) {

        PED::SET_PED_AS_GROUP_LEADER(ped1, lemoyneId, false);
        PED::SET_GROUP_FORMATION(lemoyneId, 3);
        PED::SET_GROUP_FORMATION_SPACING(lemoyneId, 2.0f, 1.0f, 2.0f);
        DECORATOR::DECOR_SET_INT(ped1, "lemoyne", 0);
    }

    if (ENTITY::DOES_ENTITY_EXIST(ped2) && !ENTITY::IS_ENTITY_DEAD(ped2)) {

        PED::SET_PED_AS_GROUP_MEMBER(ped2, lemoyneId);
        PED::SET_GROUP_FORMATION(lemoyneId, 3);
        PED::SET_GROUP_FORMATION_SPACING(lemoyneId, 2.0f, 1.0f, 2.0f);
        DECORATOR::DECOR_SET_INT(ped2, "lemoyne", 0);
    }

    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(4, REL_GANG_LEMOYNE_RAIDERS, RELGROUPHASH_PLAYER);
}

// Fun��o para gerar um Ped aleat�rio da lista e remover o modelo ap�s o uso
std::string GetRandomLemoyneHash() {

    // Semente para o gerador de n�meros aleat�rios
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Verificar se ainda h� Peds dispon�veis na lista
    if (availableLemoyneList.empty()) {
        logMessage("All Lemoyne used, resetting the list.");
        // Reiniciar a lista se todos os Peds j� foram gerados
        availableLemoyneList = lemoyne;
        usedLemoyneList.clear();  // Limpa os Peds usados
    }

    // Obter um �ndice aleat�rio
    int randomIndex = std::rand() % availableLemoyneList.size();

    // Obter o hash correspondente ao �ndice aleat�rio
    std::string selectedLemoyneHash = availableLemoyneList[randomIndex];

    logMessage("Lemoyne selected: " + selectedLemoyneHash);

    // Remover o modelo da mem�ria para evitar duplica��o
    Hash pedModel = MISC::GET_HASH_KEY(selectedLemoyneHash.c_str());
    STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(pedModel);

    // Mover o Ped selecionado para a lista de usados
    usedLemoyneList.push_back(selectedLemoyneHash);

    // Remover o Ped selecionado da lista para n�o gerar o mesmo
    availableLemoyneList.erase(availableLemoyneList.begin() + randomIndex);

    // Retornar o hash do Ped
    return selectedLemoyneHash;
}

// Fun��o para mover dois peds montados para uma coordenada espec�fica
void LemoyneMountSpawn() {

    // ID do jogador
    Player player = PLAYER::PLAYER_ID();
    Ped playerPed = PLAYER::PLAYER_PED_ID();

    // check if player ped exists and control is on (e.g. not in a cutscene)
    if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !PLAYER::IS_PLAYER_CONTROL_ON(player) || !PLAYER::IS_PLAYER_SCRIPT_CONTROL_ON(player)) {
        return;
    }

    // Pega as coordenadas atuais do jogador
    Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);
    logMessage("Player coordinates captured.");

    // Gerar hash para os cavalos aleat�rios
    std::string randomHorse = GetRandomHorseHash();
    Hash horseHash = MISC::GET_HASH_KEY(randomHorse.c_str());

    std::string randomLemoyne = GetRandomLemoyneHash();
    Hash lemoyneHash = MISC::GET_HASH_KEY(randomLemoyne.c_str());

    // Criando os cavalos aleat�rios
    Ped horse1 = createPed(horseHash, getRandomPedPositionInRange(playerPos, 80));
    Ped horse2 = createPed(horseHash, getRandomPedPositionInRange(playerPos, 80));

    // Atribuir sela aos cavalos
    giveComboSaddleToHorse(horse1);
    giveComboSaddleToHorse(horse2);

    // Criar os peds montados em seus respectivos cavalos
    Ped ped1 = pedMount(lemoyneHash, horse1, true, playerPos, 80);
    Ped ped2 = pedMount(lemoyneHash, horse2, true, playerPos, 80);
    WAIT(1000);

    // Definir a personalidade agressiva e permitir intera��o
    lemoyneGroup(ped1, ped2);
    personalityPed(ped1);
    personalityPed(ped2);
    ConfigurePedInteraction(ped1);
    ConfigurePedInteraction(ped2);
    WAIT(1000);

    // Verifica se ambos os peds (ped e cavalo) existem e est�o vivos
    if (ENTITY::DOES_ENTITY_EXIST(ped1) && !ENTITY::IS_ENTITY_DEAD(ped1) &&
        ENTITY::DOES_ENTITY_EXIST(ped2) && !ENTITY::IS_ENTITY_DEAD(ped2)) {

        Vector3 ped1Coords = ENTITY::GET_ENTITY_COORDS(ped1, true, false);  // Pega as coordenadas atuais do ped1
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

            // Verifica se a dist�ncia est� entre 500 e 1500 metros
            if (distancia >= distanciaMinima && distancia <= distanciaMaxima) {
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

                TASK::TASK_MOVE_FOLLOW_ROAD_USING_NAVMESH(ped1, 1.001f, x, y, z, 0);
                ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(ped1, true);


                // Mover o cavalo diretamente para a coordenada de destino
                invoke<Void>(0x79482C12482A860D, ped2, 1.001f, ped2, 0);
                ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(ped2, true);
            }
        }
        else {
            // Se o ponto n�o for naveg�vel, exclui os pedestres e cavalos e reinicia o script
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
            LemoyneMountSpawn();
        }
    }

    // Adicionando entidades � lista global
    globalEntityList.push_back(ped1);
    globalEntityList.push_back(ped2);
    globalEntityList.push_back(horse1);
    globalEntityList.push_back(horse2);
    pedList.push_back(ped1); // Adicionar o ped � lista
    pedBlips[ped1] = NULL;   // Inicializa o blip do ped como NULL
}