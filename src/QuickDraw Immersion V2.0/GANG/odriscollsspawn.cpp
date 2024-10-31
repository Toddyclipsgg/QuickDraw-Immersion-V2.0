#include "..\header.h"

std::vector<std::string> availableOdriscollList = odriscoll;  // Lista de Peds disponíveis para gerar
std::vector<std::string> usedOdriscollList;  // Lista de Peds já utilizados


// Função auxiliar para normalizar um vetor
Vector3 NormalizeVector(Vector3 vec)
{
    float magnitude = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    if (magnitude != 0)
    {
        vec.x /= magnitude;
        vec.y /= magnitude;
        vec.z /= magnitude;
    }
    return vec;
}

//quick function to get distance between 2 points: eg - if (distanceBetween(coordsOf(player), targetCoords) < 50)
float distanceBetween1(Vector3 A, Vector3 B) {
    return MISC::GET_DISTANCE_BETWEEN_COORDS(A.x, A.y, A.z, B.x, B.y, B.z, 1);
}

void odriscollGroup(Ped ped1, Ped ped2) {
    int odriscollId = PED::CREATE_GROUP(0);

    if (ENTITY::DOES_ENTITY_EXIST(ped1) && !ENTITY::IS_ENTITY_DEAD(ped1)) {

        PED::SET_PED_AS_GROUP_LEADER(ped1, odriscollId, false);
        PED::SET_GROUP_FORMATION(odriscollId, 3);
        PED::SET_GROUP_FORMATION_SPACING(odriscollId, 2.0f, 1.0f, 2.0f);
        DECORATOR::DECOR_SET_INT(ped1, "odriscolls", 0);
    }

    if (ENTITY::DOES_ENTITY_EXIST(ped2) && !ENTITY::IS_ENTITY_DEAD(ped2)) {

        PED::SET_PED_AS_GROUP_MEMBER(ped2, odriscollId);
        PED::SET_GROUP_FORMATION(odriscollId, 3);
        PED::SET_GROUP_FORMATION_SPACING(odriscollId, 2.0f, 1.0f, 2.0f);
        DECORATOR::DECOR_SET_INT(ped2, "odriscolls", 0);
    }

    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, REL_GANG_ODRISCOLL, RELGROUPHASH_PLAYER);
}

void odriscollGroupVehicle(Ped ped1, Ped ped2) {
    int odriscollId = PED::CREATE_GROUP(0);

    if (ENTITY::DOES_ENTITY_EXIST(ped1) && !ENTITY::IS_ENTITY_DEAD(ped1)) {

        PED::SET_PED_AS_GROUP_LEADER(ped1, odriscollId, false);
        DECORATOR::DECOR_SET_INT(ped1, "odriscolls", 0);
    }

    if (ENTITY::DOES_ENTITY_EXIST(ped2) && !ENTITY::IS_ENTITY_DEAD(ped2)) {

        PED::SET_PED_AS_GROUP_MEMBER(ped2, odriscollId);
        DECORATOR::DECOR_SET_INT(ped2, "odriscolls", 0);
    }

    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, REL_GANG_ODRISCOLL, RELGROUPHASH_PLAYER);
}

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

void OdriscollsMountSpawn() {
    
    // Distância máxima permitida (100 metros)
    double maxDistance = 500.0;

    // Verificar se o jogador está a 100 metros de qualquer uma das localizações
    if (MISC::GET_MISSION_FLAG() || isPlayerWithinDistance(maxDistance)) {
        logMessage("Player is within 100 meters of a camp or on a mission.");
        return;
    }
    else {
        logMessage("Player is not within 100 meters of any camp or on a mission.");
    }

    // ID do jogador
    Player player = PLAYER::PLAYER_ID();
    Ped playerPed = PLAYER::PLAYER_PED_ID();

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
    odriscollGroup(ped1, ped2);
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
            WAIT(100);
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
                TASK::TASK_MOVE_FOLLOW_ROAD_USING_NAVMESH(ped1, 1.250f, x, y, z, 0);
                ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(ped1, true);

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
                        TASK::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(ped2, ped1, 0.0f, -safeDistance, 0.0f, 1.250f, -1, 0.500f, true, false, false, false, false, false);
                        ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(ped2, true);
                    }
                }
                else {
                    // Se ped2 estiver muito perto, desacelere para evitar bloqueio
                    logMessage("Ped 2 is too close to Ped 1, standing still to adjust.");
                    TASK::TASK_STAND_STILL(ped2, 500); // Espera 500ms para manter distância
                }
            }
            else {
                logMessage("NavMesh path query failed for ped1. Deleting entities and restarting.");

                ENTITY::DELETE_ENTITY(&ped1);
                ENTITY::DELETE_ENTITY(&ped2);
                ENTITY::DELETE_ENTITY(&horse1);
                ENTITY::DELETE_ENTITY(&horse2);
                WAIT(2000);
                OdriscollsMountSpawn();
            }
        }
    }
    // Adicionando entidades à lista global
    globalEntityList.push_back(ped1);
    globalEntityList.push_back(ped2);
    vehicleEntityList.push_back(horse1);
    vehicleEntityList.push_back(horse2);
}

/*
void OdriscollsVehicleSpawn()
{
    // Distância máxima permitida (100 metros)
    double maxDistance = 500.0;

    // Verificar se o jogador está a 100 metros de qualquer uma das localizações
    if (MISC::GET_MISSION_FLAG() || isPlayerWithinDistance(maxDistance)) {
        logMessage("Player is within 100 meters of a camp or on a mission.");
        return;
    }
    else {
        logMessage("Player is not within 100 meters of any camp or on a mission.");
    }

    // ID do jogador
    Player player = PLAYER::PLAYER_ID();
    Ped playerPed = PLAYER::PLAYER_PED_ID();

    // Captura as coordenadas do jogador
    Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);
    logMessage("Player coordinates captured.");

    // Distância desejada
    float distance = 60.0f;

    // Posição inicial de geração
    Vector3 spawnPos = { playerPos.x + distance, playerPos.y, playerPos.z };

    // Coordenadas e orientação da estrada
    Vector3 roadPos;
    float roadHeading;

    // Localiza o nó de estrada mais próximo com a orientação da estrada
    if (PATHFIND::GET_CLOSEST_VEHICLE_NODE_WITH_HEADING(spawnPos.x, spawnPos.y, spawnPos.z, &roadPos, &roadHeading, 1, 3.0f, 0)) {
        logMessage("Road node located.");

        // Vetor de direção relativo ao jogador para garantir a orientação correta
        Vector3 directionVec = { spawnPos.x - roadPos.x, spawnPos.y - roadPos.y, 0.0f };
        directionVec = NormalizeVector(directionVec);

        Vector3 roadDir = { -BUILTIN::SIN(roadHeading), BUILTIN::COS(roadHeading), 0.0f };

        // Verifica a direção; se necessário, ajusta o ângulo do veículo
        if (distanceBetween1(directionVec, roadDir) < 0.0f) {
            roadHeading += 60.0f;
        }
    }
    else {
        logMessage("Failed to locate a road node for vehicle spawn.");
    }

    if (IsSpawnRestricted(roadPos)) {
        logMessage("Spawn blocked due to proximity to restricted location.");
        OdriscollsVehicleSpawn();
        return; // *Spawn* é proibido, então a função termina aqui
    }

#pragma warning(disable:4267)
    std::vector<int> wagon = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };  // Índices das funções

    if (wagon.empty()) {
        return;
    }

    // Embaralha as funções

    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> dist(0, wagon.size() - 1);
    int indexChosen = wagon[dist(g)];
    Entity vehicle;

    // Executa as funções conforme a ordem embaralhada
    switch (indexChosen) {
    case 1:
        vehicle = spawnVehicle(VehicleHash::Gatchuck, roadPos, roadHeading);
        attachCargoToVehicle(vehicle, VehicleCargoHash::UtillwagExplosives);
        break;
    case 2:
        vehicle = spawnVehicle(VehicleHash::Wagon04X, roadPos, roadHeading);
        attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage2);
        break;
    case 3:
        vehicle = spawnVehicle(VehicleHash::Wagon02X, roadPos, roadHeading);
        attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage3);
        break;
    case 4:
        vehicle = spawnVehicle(VehicleHash::Gatchuck, roadPos, roadHeading);
        attachCargoToVehicle(vehicle, VehicleCargoHash::TNT);
        break;
    case 5:
        vehicle = spawnVehicle(VehicleHash::Wagon04X, roadPos, roadHeading);
        attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage3);
        break;
    case 6:
        vehicle = spawnVehicle(VehicleHash::Wagon02X, roadPos, roadHeading);
        attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage1);
        break;
    case 7:
        vehicle = spawnVehicle(VehicleHash::Gatchuck2, roadPos, roadHeading);
        attachCargoToVehicle(vehicle, VehicleCargoHash::ArmyCargo);
        break;
    case 8:
        vehicle = spawnVehicle(VehicleHash::Wagon04X, roadPos, roadHeading);
        attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage1);
        break;
    case 9:
        vehicle = spawnVehicle(VehicleHash::Wagon02X, roadPos, roadHeading);
        attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage2);
        break;
    default:
        logMessage("Invalid group index.");
        break;
    }
#pragma warning(default:4267)

    // Gerar hash para os O'Driscoll aleatórios
    std::string randomOdriscoll1 = GetRandomOdriscollHash();
    Hash odriscollHash1 = MISC::GET_HASH_KEY(randomOdriscoll1.c_str());
    std::string randomOdriscoll2 = GetRandomOdriscollHash();
    Hash odriscollHash2 = MISC::GET_HASH_KEY(randomOdriscoll2.c_str());

    Ped ped1 = createPed(odriscollHash1, roadPos);
    Ped ped2 = createPed(odriscollHash2, roadPos);

    odriscollGroupVehicle(ped1, ped2);
    personalityPed(ped1);
    personalityPed(ped2);
    ConfigurePedVehicleInteraction(ped1);
    ConfigurePedVehicleInteraction(ped2);
    ManagePedBlip(vehicle, player);

    PED::SET_PED_INTO_VEHICLE(ped1, vehicle, -1);
    PED::SET_PED_INTO_VEHICLE(ped2, vehicle, -2);

    // Verifica se ambos os peds (ped e cavalo) existem e estão vivos
    if (ENTITY::DOES_ENTITY_EXIST(ped1) && !ENTITY::IS_ENTITY_DEAD(ped1) &&
        ENTITY::DOES_ENTITY_EXIST(vehicle) && !ENTITY::IS_ENTITY_DEAD(vehicle)) {
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
            WAIT(100);
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
                TASK::TASK_VEHICLE_DRIVE_TO_DESTINATION(ped1, vehicle, x, y, z, 3.0f, 2621463, 6, 3.0f, 5.0f, false);

            }
            else {
                logMessage("NavMesh path query failed for ped1. Deleting entities and restarting.");

                ENTITY::DELETE_ENTITY(&ped1);
                ENTITY::DELETE_ENTITY(&ped2);
                ENTITY::DELETE_ENTITY(&vehicle);
                WAIT(2000);
                OdriscollsVehicleSpawn();
            }
        }
    }
    globalEntityList.push_back(ped1);
    globalEntityList.push_back(ped2);
    vehicleEntityList.push_back(vehicle);
}
*/

void OdriscollsVehicleSpawn()
{
    constexpr double MAX_DISTANCE = 500.0;
    if (MISC::GET_MISSION_FLAG() || isPlayerWithinDistance(MAX_DISTANCE)) {
        logMessage("Player is within 100 meters of a camp or on a mission.");
        return;
    }
    logMessage("Player is not within 100 meters of any camp or on a mission.");

    Ped ped1 = 0;
    Player player = PLAYER::PLAYER_ID();
    Ped playerPed = PLAYER::PLAYER_PED_ID();
    Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);

    constexpr float SPAWN_DISTANCE = 60.0f;
    Vector3 spawnPos = { playerPos.x + SPAWN_DISTANCE, playerPos.y, playerPos.z };
    Vector3 roadPos;
    float roadHeading;

    if (PATHFIND::GET_CLOSEST_VEHICLE_NODE_WITH_HEADING(spawnPos.x, spawnPos.y, spawnPos.z, &roadPos, &roadHeading, 1, 3.0f, 0)) {
        logMessage("Road node located.");

        Vector3 directionVec = NormalizeVector({ spawnPos.x - roadPos.x, spawnPos.y - roadPos.y, 0.0f });
        Vector3 roadDir = { -BUILTIN::SIN(roadHeading), BUILTIN::COS(roadHeading), 0.0f };

        if (distanceBetween1(directionVec, roadDir) < 0.0f) {
            roadHeading += 60.0f;
        }
    }
    else {
        logMessage("Failed to locate a road node for vehicle spawn.");
    }

    if (IsSpawnRestricted(roadPos)) {
        logMessage("Spawn blocked due to proximity to restricted location.");
        OdriscollsVehicleSpawn();
        return;
    }

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
        WAIT(100);
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
        }
        else {
            OdriscollsVehicleSpawn();
            return;
        }
    }

    static std::vector<int> wagonIndices = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::mt19937 gen(std::random_device{}());
#pragma warning(disable:4267)
    int indexChosen = wagonIndices[std::uniform_int_distribution<>(0, wagonIndices.size() - 1)(gen)];
#pragma warning(default:4267)
    Entity vehicle = 0;  // Inicializa vehicle como inválido, em vez de nullptr


    switch (indexChosen) {
    case 1: vehicle = spawnVehicle(VehicleHash::Gatchuck, roadPos, roadHeading); attachCargoToVehicle(vehicle, VehicleCargoHash::UtillwagExplosives); break;
    case 2: vehicle = spawnVehicle(VehicleHash::Wagon04X, roadPos, roadHeading); attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage2); break;
    case 3: vehicle = spawnVehicle(VehicleHash::Wagon02X, roadPos, roadHeading); attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage3); break;
    case 4: vehicle = spawnVehicle(VehicleHash::Gatchuck, roadPos, roadHeading); attachCargoToVehicle(vehicle, VehicleCargoHash::TNT); break;
    case 5: vehicle = spawnVehicle(VehicleHash::Wagon04X, roadPos, roadHeading); attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage3); break;
    case 6: vehicle = spawnVehicle(VehicleHash::Wagon02X, roadPos, roadHeading); attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage1); break;
    case 7: vehicle = spawnVehicle(VehicleHash::Gatchuck2, roadPos, roadHeading); attachCargoToVehicle(vehicle, VehicleCargoHash::ArmyCargo); break;
    case 8: vehicle = spawnVehicle(VehicleHash::Wagon04X, roadPos, roadHeading); attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage1); break;
    case 9: vehicle = spawnVehicle(VehicleHash::Wagon02X, roadPos, roadHeading); attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage2); break;
    default: logMessage("Invalid group index."); break;
    }

    ped1 = createPed(MISC::GET_HASH_KEY(GetRandomOdriscollHash().c_str()), roadPos);
    Ped ped2 = createPed(MISC::GET_HASH_KEY(GetRandomOdriscollHash().c_str()), roadPos);

    odriscollGroupVehicle(ped1, ped2);
    personalityPed(ped1);
    personalityPed(ped2);
    ConfigurePedVehicleInteraction(ped1);
    ConfigurePedVehicleInteraction(ped2);
    ManagePedBlip(vehicle, player);

    PED::SET_PED_INTO_VEHICLE(ped1, vehicle, -1);
    PED::SET_PED_INTO_VEHICLE(ped2, vehicle, -2);

    if (ENTITY::DOES_ENTITY_EXIST(ped1) && !ENTITY::IS_ENTITY_DEAD(ped1) &&
        ENTITY::DOES_ENTITY_EXIST(vehicle) && !ENTITY::IS_ENTITY_DEAD(vehicle)) {

        float x = destino.second.x;
        float y = destino.second.y;
        float z = destino.second.z;
        
        if (PATHFIND::IS_POINT_ON_ROAD(x, y, z, ped1)) {
            logMessage("Point is navigable, moving Ped 1.");
            TASK::TASK_VEHICLE_DRIVE_TO_DESTINATION(ped1, vehicle, x, y, z, 3.0f, 2621463, 6, 3.0f, 5.0f, false);
        }
        else {
            logMessage("NavMesh path query failed for ped1. Deleting entities and restarting.");
            ENTITY::DELETE_ENTITY(&ped1);
            ENTITY::DELETE_ENTITY(&ped2);
            ENTITY::DELETE_ENTITY(&vehicle);
            WAIT(2000);
            OdriscollsVehicleSpawn();
        }
    }

    globalEntityList.push_back(ped1);
    globalEntityList.push_back(ped2);
    vehicleEntityList.push_back(vehicle);
}
