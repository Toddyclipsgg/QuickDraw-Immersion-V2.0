#include "..\header.h"

// Lista de Peds disponíveis para gerar
std::vector<std::string> availableOdriscollList = odriscoll;
std::vector<std::string> usedOdriscollList;  // Lista de Peds já utilizados

// --------------------------------------------------------------------------------------------------------------

// Estrutura para representar um grupo de modelos
struct GrupoModelos
{
    const char* nomeGrupo;
    std::vector<std::string> modelos;
};

// Função para obter o grupo específico
static GrupoModelos ObterGrupo(int index)
{
    switch(index) {
        case 0:
            return {
                "Odriscoll",
                {
                    "G_M_M_UNIDUSTER_03",
                    "U_M_M_UNIDUSTERHENCHMAN_03",
                    "G_M_M_UNIDUSTER_04", 
                    "U_M_M_UNIDUSTERHENCHMAN_02",
                    "G_M_M_UNIDUSTER_02",
                    "U_M_M_FATDUSTER_01",
                    "U_M_M_UNIDUSTERHENCHMAN_01",
                    "G_M_M_UNIDUSTER_01",
                    "CS_DISGUISEDDUSTER_01",
                    "U_M_M_UNIDUSTERLEADER_01",
                    "CS_DISGUISEDDUSTER_03",
                    "G_M_M_UNIDUSTER_05",
                    "CS_DISGUISEDDUSTER_02"
                }
            };
        case 1:
            return {
                "ExConfederados",
                {
                    "G_M_Y_UNIEXCONFEDS_02",
                    "G_M_Y_UNIEXCONFEDS_01",
                    "G_M_O_UNIEXCONFEDS_01",
                    "CS_EXCONFEDSLEADER_01",
                    "U_M_M_UNIEXCONFEDSBOUNTY_01"
                }
            };
        case 2:
            return {
                "Skinner Brothers",
                {
                    "U_M_M_BHT_SKINNERSEARCH",
                    "G_M_M_UNIMOUNTAINMEN_01",
                    "U_M_M_BHT_SKINNERBROTHER"
                }
            };
        case 3:
            return {
                "Ranchers",
                {
                    "G_M_M_UNIRANCHERS_01",
                    "CS_LARAMIE"
                }
            };
        case 4:
            return {
                "Banditos",
                {
                    "U_M_M_BHT_BANDITOSHACK",
                    "U_M_M_BHT_BANDITOMINE",
                    "G_M_M_UNIBANDITOS_01"
                }
            };
        case 5:
            return {
                "Bronte",
                {
                    "U_M_M_NBXBRONTEGOON_01",
                    "U_M_M_NBXBRONTESECFORM_01",
                    "U_M_M_NBXBRONTEASC_01",
                    "G_M_M_UNIBRONTEGOONS_01"
                }
            };
        default:
            return {"", {}};
    }
}

// Função para gerar duas strings diferentes do mesmo grupo de forma aleatória
std::pair<std::string, std::string> GerarDuasStringsDoMesmoGrupo() {
    const int NUM_GROUPS = 6; // Total de grupos disponíveis
    int grupoIndex = rand() % NUM_GROUPS; // Seleção aleatória do índice do grupo
    GrupoModelos grupo = ObterGrupo(grupoIndex);
    
    if (grupo.modelos.size() < 2) {
        logMessage("Não há modelos suficientes no grupo " + std::string(grupo.nomeGrupo));
        return {"", ""};
    }

    int firstIndex = rand() % grupo.modelos.size();
    int secondIndex;
    do {
        secondIndex = rand() % grupo.modelos.size();
    } while (secondIndex == firstIndex);

    return {grupo.modelos[firstIndex], grupo.modelos[secondIndex]};
}

// --------------------------------------------------------------------------------------------------------------

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

// Função para calcular a distância entre dois pontos
float CalculateDistance(Vector3 A, Vector3 B) {
    return MISC::GET_DISTANCE_BETWEEN_COORDS(A.x, A.y, A.z, B.x, B.y, B.z, 1);
}

// Função para criar um grupo de O'Driscolls
void createOdriscollGroup(Ped ped1, Ped ped2, bool isVehicleGroup) {
    int odriscollId = PED::CREATE_GROUP(0);

    if (ENTITY::DOES_ENTITY_EXIST(ped1) && !ENTITY::IS_ENTITY_DEAD(ped1)) {
        PED::SET_PED_AS_GROUP_LEADER(ped1, odriscollId, false);
        DECORATOR::DECOR_SET_INT(ped1, "odriscolls", 0);
    }

    if (ENTITY::DOES_ENTITY_EXIST(ped2) && !ENTITY::IS_ENTITY_DEAD(ped2)) {
        PED::SET_PED_AS_GROUP_MEMBER(ped2, odriscollId);
        DECORATOR::DECOR_SET_INT(ped2, "odriscolls", 0);
    }

    if (!isVehicleGroup) {
        PED::SET_GROUP_FORMATION(odriscollId, 3);
        PED::SET_GROUP_FORMATION_SPACING(odriscollId, 2.0f, 1.0f, 2.0f);
    }

    // Configurar relacionamento entre grupos
    Hash gang1 = PED::GET_PED_RELATIONSHIP_GROUP_HASH(ped1);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, gang1, RELGROUPHASH_PLAYER);
    Hash gang2 = PED::GET_PED_RELATIONSHIP_GROUP_HASH(ped2);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, gang2, RELGROUPHASH_PLAYER);
}

// Função para gerar um Ped aleatório da lista e remover o modelo após o uso
std::string GetRandomOdriscollHash() {
    if (availableOdriscollList.empty()) {
        logMessage("All O'Driscolls used, resetting the list.");
        availableOdriscollList = odriscoll;
        usedOdriscollList.clear();
    }

    int randomIndex = rand() % availableOdriscollList.size();
    std::string selectedOdriscollHash = availableOdriscollList[randomIndex];

    logMessage("O'Driscoll selected: " + selectedOdriscollHash);

    Hash pedModel = MISC::GET_HASH_KEY(selectedOdriscollHash.c_str());
    STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(pedModel);

    usedOdriscollList.push_back(selectedOdriscollHash);
    availableOdriscollList.erase(availableOdriscollList.begin() + randomIndex);

    return selectedOdriscollHash;
}

// Função para verificar se o spawn é permitido
bool IsSpawnAllowed(double maxDistance) {
    if (MISC::GET_MISSION_FLAG() || isPlayerWithinDistance(maxDistance)) {
        logMessage("Player is within range of a camp or on a mission.");
        return false;
    }
    return true;
}

// Função para encontrar um destino válido
bool FindValidDestination(Vector3 origin, float minDistance, float maxDistance, Vector3& outDestination) {
    int maxAttempts = 25;
    for (int i = 0; i < maxAttempts; ++i) {
        WAIT(100);
        auto destino = GetRandomCoord();
        Vector3 destination = destino.second; // Certifique-se de que destino.second seja do tipo Vector3
        float distance = CalculateDistance(origin, destination);
        logMessage("Attempt " + std::to_string(i) + ": Checking distance for generated coordinate.");
        if (distance >= minDistance && distance <= maxDistance) {
            logMessage("Coordinate found within valid range.");
            outDestination = destino.second;
            return true;
        }
    }
    return false;
}

// Função para mover o grupo de O'Driscolls a pé até o destino
void MoveFootGroupToDestination(Ped leader, Ped follower, Vector3 destination) {
    if (PATHFIND::IS_POINT_ON_ROAD(destination.x, destination.y, destination.z, leader)) {
        logMessage("Point is navigable, moving group.");

        TASK::TASK_MOVE_FOLLOW_ROAD_USING_NAVMESH(leader, 0.500f, destination.x, destination.y, destination.z, 0);
        ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(leader, true);

        float safeDistance = 2.0f;

        if (!ENTITY::IS_ENTITY_DEAD(follower)) {
            TASK::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(follower, leader, 0.0f, -safeDistance, 0.0f, 0.500f, -1, 0.500f, true, false, false, false, false, false);
            ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(follower, true);
        }
    } else {
        logMessage("NavMesh path query failed for foot group. Deleting entities and restarting.");
        ENTITY::DELETE_ENTITY(&leader);
        ENTITY::DELETE_ENTITY(&follower);
        // Handle deletion of other entities if necessary
    }
}

// Função para mover o ped até o destino
void MoveGroupToDestination(Ped leader, Ped follower, Vector3 destination) {
    if (PATHFIND::IS_POINT_ON_ROAD(destination.x, destination.y, destination.z, leader)) {
        logMessage("Point is navigable, moving group.");

        TASK::TASK_MOVE_FOLLOW_ROAD_USING_NAVMESH(leader, 1.250f, destination.x, destination.y, destination.z, 0);
        ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(leader, true);

        float safeDistance = 2.0f;

        if (!ENTITY::IS_ENTITY_DEAD(follower)) {
            TASK::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(follower, leader, 0.0f, -safeDistance, 0.0f, 1.250f, -1, 0.500f, true, false, false, false, false, false);
            ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(follower, true);
        }
    } else {
        logMessage("NavMesh path query failed. Deleting entities and restarting.");
        ENTITY::DELETE_ENTITY(&leader);
        ENTITY::DELETE_ENTITY(&follower);
        // Handle deletion of other entities if necessary
    }
}

// Função para gerar e configurar um grupo de O'Driscolls montados
void OdriscollsMountSpawn() {
    const double maxDistance = 500.0;
    if (!IsSpawnAllowed(maxDistance)) {
        return;
    }

    Ped playerPed = PLAYER::PLAYER_PED_ID();
    Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);
    logMessage("Player coordinates captured.");

    std::string randomHorse1 = GetRandomHorseHash();
    Hash horseHash1 = MISC::GET_HASH_KEY(randomHorse1.c_str());
    std::string randomHorse2 = GetRandomHorseHash();
    Hash horseHash2 = MISC::GET_HASH_KEY(randomHorse2.c_str());

    /*std::string randomOdriscoll1 = GetRandomOdriscollHash();
    Hash odriscollHash1 = MISC::GET_HASH_KEY(randomOdriscoll1.c_str());
    std::string randomOdriscoll2 = GetRandomOdriscollHash();
    Hash odriscollHash2 = MISC::GET_HASH_KEY(randomOdriscoll2.c_str());*/

    Ped horse1 = QuickDrawImmersion::createPed(horseHash1, getRandomPedPositionInRange(playerPos, 80));
    Ped horse2 = QuickDrawImmersion::createPed(horseHash2, getRandomPedPositionInRange(playerPos, 80));
    logMessage("Horses created.");

    QuickDrawImmersion::giveComboSaddleToHorse(horse1);
    QuickDrawImmersion::giveComboSaddleToHorse(horse2);

    Ped ped1;
    Ped ped2;

    /*Ped ped1 = QuickDrawImmersion::pedMount(odriscollHash1, horse1, true, playerPos, 80);
    Ped ped2 = QuickDrawImmersion::pedMount(odriscollHash2, horse2, true, playerPos, 80);*/

    int grupoIndex = 0; // Índice do grupo desejado
    auto strings = GerarDuasStringsDoMesmoGrupo();
    if (!strings.first.empty() && !strings.second.empty()) {
        Hash gang1 = MISC::GET_HASH_KEY(strings.first.c_str());
        Hash gang2 = MISC::GET_HASH_KEY(strings.second.c_str());
        logMessage("Gang 1: " + strings.first);
        logMessage("Gang 2: " + strings.second);

        ped1 = QuickDrawImmersion::pedMount(gang1, horse1, true, playerPos, 80);
        ped2 = QuickDrawImmersion::pedMount(gang2, horse2, true, playerPos, 80);
    }
    logMessage("Peds mounted on horses.");
    WAIT(4500);

    createOdriscollGroup(ped1, ped2, false);
    personalityPed(ped1);
    personalityPed(ped2);
    ConfigurePedInteraction(ped1);
    ConfigurePedInteraction(ped2);
    logMessage("Peds personality set and interactions configured.");
    WAIT(1000);

    if (ENTITY::DOES_ENTITY_EXIST(ped1) && !ENTITY::IS_ENTITY_DEAD(ped1) &&
        ENTITY::DOES_ENTITY_EXIST(ped2) && !ENTITY::IS_ENTITY_DEAD(ped2)) {

        Vector3 ped1Coords = ENTITY::GET_ENTITY_COORDS(ped1, true, false);
        logMessage("Ped 1 coordinates captured.");

        Vector3 destination;
        const float distanciaMaxima = 1500.0f;
        const float distanciaMinima = 500.0f;

        if (FindValidDestination(ped1Coords, distanciaMinima, distanciaMaxima, destination)) {
            MoveGroupToDestination(ped1, ped2, destination);
        } else {
            logMessage("No valid destination found. Deleting entities and restarting.");
            ENTITY::DELETE_ENTITY(&ped1);
            ENTITY::DELETE_ENTITY(&ped2);
            ENTITY::DELETE_ENTITY(&horse1);
            ENTITY::DELETE_ENTITY(&horse2);
            OdriscollsMountSpawn();
            return;
        }
    }

    globalEntityList.push_back(ped1);
    globalEntityList.push_back(ped2);
    vehicleEntityList.push_back(horse1);
    vehicleEntityList.push_back(horse2);
}

// Função para gerar e configurar um grupo de O'Driscolls em um veículo
void OdriscollsVehicleSpawn() {
    const double maxDistance = 500.0;
    if (!IsSpawnAllowed(maxDistance)) {
        return;
    }

    Ped playerPed = PLAYER::PLAYER_PED_ID();
    Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);
    logMessage("Player coordinates captured.");

    float spawnDistance = 60.0f;
    Vector3 spawnPos = { playerPos.x + spawnDistance, playerPos.y, playerPos.z };

    Vector3 roadPos;
    float roadHeading;

    if (PATHFIND::GET_CLOSEST_VEHICLE_NODE_WITH_HEADING(spawnPos.x, spawnPos.y, spawnPos.z, &roadPos, &roadHeading, 1, 3.0f, 0)) {
        logMessage("Road node located.");

        Vector3 directionVec = { spawnPos.x - roadPos.x, spawnPos.y - roadPos.y, 0.0f };
        directionVec = NormalizeVector(directionVec);

        Vector3 roadDir = { -BUILTIN::SIN(roadHeading), BUILTIN::COS(roadHeading), 0.0f };

        if (CalculateDistance(directionVec, roadDir) < 0.0f) {
            roadHeading += 60.0f;
        }
    } else {
        logMessage("Failed to locate a road node for vehicle spawn.");
    }

    if (IsSpawnRestricted(roadPos)) {
        logMessage("Spawn blocked due to proximity to restricted location.");
        OdriscollsVehicleSpawn();
        return;
    }

    std::vector<int> wagonOptions = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int indexChosen = wagonOptions[rand() % wagonOptions.size()];
    Entity vehicle;

    switch (indexChosen) {
    case 1:
        vehicle = VehicleSpawner::spawnVehicle(VehicleHash::Gatchuck, roadPos, roadHeading);
        VehicleSpawner::attachCargoToVehicle(vehicle, VehicleCargoHash::UtillwagExplosives);
        break;
    case 2:
        vehicle = VehicleSpawner::spawnVehicle(VehicleHash::Wagon04X, roadPos, roadHeading);
        VehicleSpawner::attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage2);
        break;
    case 3:
        vehicle = VehicleSpawner::spawnVehicle(VehicleHash::Wagon02X, roadPos, roadHeading);
        VehicleSpawner::attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage3);
        break;
    case 4:
        vehicle = VehicleSpawner::spawnVehicle(VehicleHash::Gatchuck, roadPos, roadHeading);
        VehicleSpawner::attachCargoToVehicle(vehicle, VehicleCargoHash::TNT);
        break;
    case 5:
        vehicle = VehicleSpawner::spawnVehicle(VehicleHash::Wagon04X, roadPos, roadHeading);
        VehicleSpawner::attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage3);
        break;
    case 6:
        vehicle = VehicleSpawner::spawnVehicle(VehicleHash::Wagon02X, roadPos, roadHeading);
        VehicleSpawner::attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage1);
        break;
    case 7:
        vehicle = VehicleSpawner::spawnVehicle(VehicleHash::Gatchuck2, roadPos, roadHeading);
        VehicleSpawner::attachCargoToVehicle(vehicle, VehicleCargoHash::ArmyCargo);
        break;
    case 8:
        vehicle = VehicleSpawner::spawnVehicle(VehicleHash::Wagon04X, roadPos, roadHeading);
        VehicleSpawner::attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage1);
        break;
    case 9:
        vehicle = VehicleSpawner::spawnVehicle(VehicleHash::Wagon02X, roadPos, roadHeading);
        VehicleSpawner::attachCargoToVehicle(vehicle, VehicleCargoHash::Luggage2);
        break;
    default:
        logMessage("Invalid wagon index.");
        break;
    }

    /*std::string randomOdriscoll1 = GetRandomOdriscollHash();
    Hash odriscollHash1 = MISC::GET_HASH_KEY(randomOdriscoll1.c_str());
    std::string randomOdriscoll2 = GetRandomOdriscollHash();
    Hash odriscollHash2 = MISC::GET_HASH_KEY(randomOdriscoll2.c_str());

    Ped ped1 = QuickDrawImmersion::createPed(odriscollHash1, roadPos);
    Ped ped2 = QuickDrawImmersion::createPed(odriscollHash2, roadPos);*/

    Ped ped1;
    Ped ped2;

    int grupoIndex = 0; // Índice do grupo desejado
    auto strings = GerarDuasStringsDoMesmoGrupo();
    if (!strings.first.empty() && !strings.second.empty()) {
        Hash gang1 = MISC::GET_HASH_KEY(strings.first.c_str());
        Hash gang2 = MISC::GET_HASH_KEY(strings.second.c_str());
        logMessage("Gang 1: " + strings.first);
        logMessage("Gang 2: " + strings.second);

        ped1 = QuickDrawImmersion::createPed(gang1, roadPos);
        ped2 = QuickDrawImmersion::createPed(gang2, roadPos);
    }

    createOdriscollGroup(ped1, ped2, true);
    personalityPed(ped1);
    personalityPed(ped2);
    ConfigurePedVehicleInteraction(ped1);
    ConfigurePedVehicleInteraction(ped2);
    //ManagePedBlip(vehicle, PLAYER::PLAYER_ID());

    PED::SET_PED_INTO_VEHICLE(ped1, vehicle, -1);
    PED::SET_PED_INTO_VEHICLE(ped2, vehicle, -2);

    if (ENTITY::DOES_ENTITY_EXIST(ped1) && !ENTITY::IS_ENTITY_DEAD(ped1) &&
        ENTITY::DOES_ENTITY_EXIST(vehicle) && !ENTITY::IS_ENTITY_DEAD(vehicle)) {

        Vector3 ped1Coords = ENTITY::GET_ENTITY_COORDS(ped1, true, false);
        logMessage("Ped 1 coordinates captured.");

        Vector3 destination;
        const float distanciaMaxima = 1500.0f;
        const float distanciaMinima = 500.0f;

        if (FindValidDestination(ped1Coords, distanciaMinima, distanciaMaxima, destination)) {
            if (PATHFIND::IS_POINT_ON_ROAD(destination.x, destination.y, destination.z, ped1)) {
                logMessage("Point is navigable, moving vehicle with Ped 1.");
                TASK::TASK_VEHICLE_DRIVE_TO_DESTINATION(ped1, vehicle, destination.x, destination.y, destination.z, 3.0f, 2621463, 6, 3.0f, 5.0f, false);
            } else {
                logMessage("NavMesh path query failed. Deleting entities and restarting.");
                ENTITY::DELETE_ENTITY(&ped1);
                ENTITY::DELETE_ENTITY(&ped2);
                ENTITY::DELETE_ENTITY(&vehicle);
                OdriscollsVehicleSpawn();
                return;
            }
        } else {
            logMessage("No valid destination found. Deleting entities and restarting.");
            ENTITY::DELETE_ENTITY(&ped1);
            ENTITY::DELETE_ENTITY(&ped2);
            ENTITY::DELETE_ENTITY(&vehicle);
            OdriscollsVehicleSpawn();
            return;
        }
    }

    globalEntityList.push_back(ped1);
    globalEntityList.push_back(ped2);
    vehicleEntityList.push_back(vehicle);
}

void OdriscollsFootSpawn() {
    const double maxDistance = 500.0;
    if (!IsSpawnAllowed(maxDistance)) {
        return;
    }

    Ped playerPed = PLAYER::PLAYER_PED_ID();
    Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);
    logMessage("Player coordinates captured.");

    Ped ped1;
    Ped ped2;

    int grupoIndex = 0; // Índice do grupo desejado
    auto strings = GerarDuasStringsDoMesmoGrupo();
    if (!strings.first.empty() && !strings.second.empty()) {
        Hash gang1 = MISC::GET_HASH_KEY(strings.first.c_str());
        Hash gang2 = MISC::GET_HASH_KEY(strings.second.c_str());
        logMessage("Gang 1: " + strings.first);
        logMessage("Gang 2: " + strings.second);

        ped1 = QuickDrawImmersion::createPed(gang1, getRandomPedPositionInRange(playerPos, 80));
        ped2 = QuickDrawImmersion::createPed(gang2, getRandomPedPositionInRange(playerPos, 80));
    }
    logMessage("Peds created.");
    WAIT(4500);

    createOdriscollGroup(ped1, ped2, false);
    personalityPed(ped1);
    personalityPed(ped2);
    ConfigurePedInteraction(ped1);
    ConfigurePedInteraction(ped2);
    logMessage("Peds personality set and interactions configured.");
    WAIT(1000);

    if (ENTITY::DOES_ENTITY_EXIST(ped1) && !ENTITY::IS_ENTITY_DEAD(ped1) &&
        ENTITY::DOES_ENTITY_EXIST(ped2) && !ENTITY::IS_ENTITY_DEAD(ped2)) {

        Vector3 ped1Coords = ENTITY::GET_ENTITY_COORDS(ped1, true, false);
        logMessage("Ped 1 coordinates captured.");

        Vector3 destination;
        const float distanciaMaxima = 1500.0f;
        const float distanciaMinima = 500.0f;

        if (FindValidDestination(ped1Coords, distanciaMinima, distanciaMaxima, destination)) {
            MoveFootGroupToDestination(ped1, ped2, destination);
        } else {
            logMessage("No valid destination found. Deleting entities and restarting.");
            ENTITY::DELETE_ENTITY(&ped1);
            ENTITY::DELETE_ENTITY(&ped2);
            OdriscollsFootSpawn();
            return;
        }
    }

    globalEntityList.push_back(ped1);
    globalEntityList.push_back(ped2);
}

void SpawnMountedAssassins() {
    const double maxDistance = 500.0;
    if (!IsSpawnAllowed(maxDistance)) {
        return;
    }

    Ped playerPed = PLAYER::PLAYER_PED_ID();
    Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);
    logMessage("Player coordinates captured for assassins.");

    std::string randomHorse1 = GetRandomHorseHash();
    Hash horseHash1 = MISC::GET_HASH_KEY(randomHorse1.c_str());
    std::string randomHorse2 = GetRandomHorseHash();
    Hash horseHash2 = MISC::GET_HASH_KEY(randomHorse2.c_str());

    Ped horse1 = QuickDrawImmersion::createPed(horseHash1, getRandomPedPositionInRange(playerPos, 100));
    Ped horse2 = QuickDrawImmersion::createPed(horseHash2, getRandomPedPositionInRange(playerPos, 100));
    logMessage("Assassin horses created.");

    QuickDrawImmersion::giveComboSaddleToHorse(horse1);
    QuickDrawImmersion::giveComboSaddleToHorse(horse2);

    auto strings = GerarDuasStringsDoMesmoGrupo();
    Ped ped1, ped2;
    if (!strings.first.empty() && !strings.second.empty()) {
        Hash gang1 = MISC::GET_HASH_KEY(strings.first.c_str());
        Hash gang2 = MISC::GET_HASH_KEY(strings.second.c_str());
        logMessage("Assassin Gang 1: " + strings.first);
        logMessage("Assassin Gang 2: " + strings.second);

        ped1 = QuickDrawImmersion::pedMount(gang1, horse1, true, playerPos, 100);
        ped2 = QuickDrawImmersion::pedMount(gang2, horse2, true, playerPos, 100);
    }
    logMessage("Assassin peds mounted on horses.");
    WAIT(4500);

    createOdriscollGroup(ped1, ped2, true);
    personalityPed(ped1);
    personalityPed(ped2);
    ConfigurePedVehicleInteraction(ped1);
    ConfigurePedVehicleInteraction(ped2);
    logMessage("Assassin peds configured.");

    if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(ped1, playerPed, true)) {
        PED::SET_PED_COMBAT_ATTRIBUTES(ped1, 46, true);
        PED::SET_PED_TARGET_LOSS_RESPONSE(ped1, 1);
        TASK::TASK_COMBAT_PED(ped1, playerPed, 0, 16);
    }

    if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(ped2, playerPed, true)) {
        PED::SET_PED_COMBAT_ATTRIBUTES(ped2, 46, true);
        PED::SET_PED_TARGET_LOSS_RESPONSE(ped2, 1);
        TASK::TASK_COMBAT_PED(ped2, playerPed, 0, 16);
    }

    if (ENTITY::DOES_ENTITY_EXIST(ped1) && !ENTITY::IS_ENTITY_DEAD(ped1) &&
        ENTITY::DOES_ENTITY_EXIST(ped2) && !ENTITY::IS_ENTITY_DEAD(ped2)) {

        Vector3 ped1Coords = ENTITY::GET_ENTITY_COORDS(ped1, true, false);
        logMessage("Ped 1 coordinates captured.");

        Vector3 destination;
        const float distanciaMaxima = 1500.0f;
        const float distanciaMinima = 500.0f;

        if (FindValidDestination(ped1Coords, distanciaMinima, distanciaMaxima, destination)) {
            MoveGroupToDestination(ped1, ped2, destination);
        } else {
            logMessage("No valid destination found. Deleting entities and restarting.");
            ENTITY::DELETE_ENTITY(&ped1);
            ENTITY::DELETE_ENTITY(&ped2);
            SpawnMountedAssassins();
            return;
        }
    }

    globalEntityList.push_back(ped1);
    globalEntityList.push_back(ped2);
    vehicleEntityList.push_back(horse1);
    vehicleEntityList.push_back(horse2);
}
