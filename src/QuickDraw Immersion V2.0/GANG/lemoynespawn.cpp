#include "..\header.h"

std::vector<std::string> availableLemoyneList = lemoyne;  // Lista de Peds disponíveis para gerar
std::vector<std::string> usedLemoyneList;  // Lista de Peds já utilizados

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

    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, REL_GANG_LEMOYNE_RAIDERS, RELGROUPHASH_PLAYER);
}

std::string GetRandomLemoyneHash() {
    std::srand(static_cast<unsigned int>(std::time(0)));

    if (availableLemoyneList.empty()) {
        logMessage("All Lemoyne used, resetting the list.");
        availableLemoyneList = lemoyne;
        usedLemoyneList.clear();
    }

    int randomIndex = std::rand() % availableLemoyneList.size();
    std::string selectedLemoyneHash = availableLemoyneList[randomIndex];

    logMessage("Lemoyne selected: " + selectedLemoyneHash);

    Hash pedModel = MISC::GET_HASH_KEY(selectedLemoyneHash.c_str());
    STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(pedModel);

    usedLemoyneList.push_back(selectedLemoyneHash);
    availableLemoyneList.erase(availableLemoyneList.begin() + randomIndex);

    return selectedLemoyneHash;
}

void LemoyneMountSpawn() {
    if (MISC::GET_MISSION_FLAG()) return;

    Player player = PLAYER::PLAYER_ID();
    Ped playerPed = PLAYER::PLAYER_PED_ID();
    Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);
    logMessage("Player coordinates captured.");
    WAIT(500);

    constexpr float MAX_SPAWN_DISTANCE = 1500.0f;
    constexpr float MIN_SPAWN_DISTANCE = 500.0f;
    constexpr int MAX_ATTEMPTS = 25;

    Vector3 ped1Coords = ENTITY::GET_ENTITY_COORDS(0, true, false);
    std::pair<std::string, Vector3> destino;
    float distancia = 0.0f;
    int tentativaAtual = 0;

    while (tentativaAtual < MAX_ATTEMPTS) {
        WAIT(200);
        destino = GetRandomCoord();
        distancia = CalculateDistance(ped1Coords, destino.second);
        logMessage("Attempt " + std::to_string(tentativaAtual) + ": Checking distance for generated coordinate.");

        if (distancia >= MIN_SPAWN_DISTANCE && distancia <= MAX_SPAWN_DISTANCE) {
            logMessage("Coordinate found within valid range.");
            break;
        }
        tentativaAtual++;
    }

    if (distancia >= MIN_SPAWN_DISTANCE && distancia <= MAX_SPAWN_DISTANCE &&
        PATHFIND::IS_POINT_ON_ROAD(destino.second.x, destino.second.y, destino.second.z, 0)) {
        logMessage("Point is navigable.");
    } else {
        spawnGroupManager();
        return;
    }

    Hash horseHash1 = MISC::GET_HASH_KEY(GetRandomHorseHash().c_str());
    Hash horseHash2 = MISC::GET_HASH_KEY(GetRandomHorseHash().c_str());
    Hash lemoyneHash1 = MISC::GET_HASH_KEY(GetRandomLemoyneHash().c_str());
    Hash lemoyneHash2 = MISC::GET_HASH_KEY(GetRandomLemoyneHash().c_str());

    Ped horse1 = createPed(horseHash1, getRandomPedPositionInRange(playerPos, 80));
    Ped horse2 = createPed(horseHash2, getRandomPedPositionInRange(playerPos, 80));
    logMessage("Horses created.");
    WAIT(600);
    giveComboSaddleToHorse(horse1);
    giveComboSaddleToHorse(horse2);

    Ped ped1 = pedMount(lemoyneHash1, horse1, true, playerPos, 80);
    Ped ped2 = pedMount(lemoyneHash2, horse2, true, playerPos, 80);
    logMessage("Peds mounted on horses.");
    WAIT(4500);

    lemoyneGroup(ped1, ped2);
    personalityPed(ped1);
    personalityPed(ped2);
    ConfigurePedInteraction(ped1);
    ConfigurePedInteraction(ped2);
    logMessage("Peds personality set and interactions configured.");
    WAIT(1000);

    if (ENTITY::DOES_ENTITY_EXIST(ped1) && !ENTITY::IS_ENTITY_DEAD(ped1) &&
        ENTITY::DOES_ENTITY_EXIST(ped2) && !ENTITY::IS_ENTITY_DEAD(ped2)) {

        if (PATHFIND::IS_POINT_ON_ROAD(destino.second.x, destino.second.y, destino.second.z, ped1)) {
            logMessage("Point is navigable, moving Ped 1.");
            WAIT(500);
            TASK::TASK_MOVE_FOLLOW_ROAD_USING_NAVMESH(ped1, 1.250f, destino.second.x, destino.second.y, destino.second.z, 0);
            ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(ped1, true);

            constexpr float SAFE_DISTANCE = 2.0f;
            constexpr float MIN_FOLLOW_DISTANCE = 1.5f;
            Vector3 ped2Position = ENTITY::GET_ENTITY_COORDS(ped2, true, false);
            float followDistance = CalculateDistance(ENTITY::GET_ENTITY_COORDS(ped1, true, false), ped2Position);
            logMessage("Ped 1 and Ped 2 distance calculated.");

            if (followDistance > MIN_FOLLOW_DISTANCE) {
                logMessage("Ped 2 following Ped 1.");
                if (!ENTITY::IS_ENTITY_DEAD(ped2)) {
                    TASK::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(ped2, ped1, 0.0f, -SAFE_DISTANCE, 0.0f, 1.250f, -1, 0.500f, true, false, false, false, false, false);
                    ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(ped2, true);
                }
            } else {
                logMessage("Ped 2 is too close to Ped 1, standing still to adjust.");
                TASK::TASK_STAND_STILL(ped2, 500);
            }
        } else {
            logMessage("NavMesh path query failed for ped1. Deleting entities and restarting.");
            ENTITY::DELETE_ENTITY(&ped1);
            ENTITY::DELETE_ENTITY(&ped2);
            ENTITY::DELETE_ENTITY(&horse1);
            ENTITY::DELETE_ENTITY(&horse2);
            spawnGroupManager();
            return;
        }
    }

    globalEntityList.push_back(ped1);
    globalEntityList.push_back(ped2);
    vehicleEntityList.push_back(horse1);
    vehicleEntityList.push_back(horse2);
    WAIT(600);
}

