#include "..\header.h"

std::vector<std::string> availableSkinnerList = skinner;  // List of available Skinners to spawn
std::vector<std::string> usedSkinnerList;                 // List of Skinners already used

void skinnerGroup(Ped leader, Ped member) {
    int skinnerGroupId = PED::CREATE_GROUP(0);

    if (ENTITY::DOES_ENTITY_EXIST(leader) && !ENTITY::IS_ENTITY_DEAD(leader)) {
        PED::SET_PED_AS_GROUP_LEADER(leader, skinnerGroupId, false);
        PED::SET_GROUP_FORMATION(skinnerGroupId, 12);
        PED::SET_GROUP_FORMATION_SPACING(skinnerGroupId, 2.0f, 1.0f, 2.0f);
        DECORATOR::DECOR_SET_INT(leader, "skinner", 0);
    }

    if (ENTITY::DOES_ENTITY_EXIST(member) && !ENTITY::IS_ENTITY_DEAD(member)) {
        PED::SET_PED_AS_GROUP_MEMBER(member, skinnerGroupId);
        PED::SET_GROUP_FORMATION(skinnerGroupId, 12);
        PED::SET_GROUP_FORMATION_SPACING(skinnerGroupId, 2.0f, 1.0f, 2.0f);
        DECORATOR::DECOR_SET_INT(member, "skinner", 0);
    }

    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, REL_GANG_SKINNER_BROTHERS, RELGROUPHASH_PLAYER);
}

std::string GetRandomSkinnerHash() {
    std::srand(static_cast<unsigned int>(std::time(0)));

    if (availableSkinnerList.empty()) {
        logMessage("All Skinner Brothers used, resetting the list.");
        availableSkinnerList = skinner;
        usedSkinnerList.clear();
    }

    int randomIndex = std::rand() % availableSkinnerList.size();
    std::string selectedSkinnerHash = availableSkinnerList[randomIndex];

    logMessage("Skinner Brother selected: " + selectedSkinnerHash);

    usedSkinnerList.push_back(selectedSkinnerHash);
    availableSkinnerList.erase(availableSkinnerList.begin() + randomIndex);

    return selectedSkinnerHash;
}

void SkinnerMountSpawn() {
    if (MISC::GET_MISSION_FLAG()) return;

    Ped playerPed = PLAYER::PLAYER_PED_ID();
    Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);
    logMessage("Player coordinates captured.");

    constexpr float MAX_SPAWN_DISTANCE = 1500.0f;
    constexpr float MIN_SPAWN_DISTANCE = 500.0f;
    constexpr int MAX_ATTEMPTS = 25;

    Vector3 spawnCoords;
    int currentAttempt = 0;
    bool validSpawnPoint = false;

    while (currentAttempt < MAX_ATTEMPTS) {
        WAIT(200);
        auto destination = GetRandomCoord();
        float distance = CalculateDistance1(playerPos, destination.second);

        logMessage("Attempt " + std::to_string(currentAttempt) + ": Checking distance for generated coordinate.");

        if (distance >= MIN_SPAWN_DISTANCE && distance <= MAX_SPAWN_DISTANCE &&
            PATHFIND::IS_POINT_ON_ROAD(destination.second.x, destination.second.y, destination.second.z, 0)) {
            logMessage("Valid spawn coordinate found.");
            spawnCoords = destination.second;
            validSpawnPoint = true;
            break;
        }
        currentAttempt++;
    }

    if (!validSpawnPoint) {
        logMessage("Failed to find a valid spawn point.");
        return;
    }

    Hash horseHash1 = MISC::GET_HASH_KEY(GetRandomHorseHash().c_str());
    Hash horseHash2 = MISC::GET_HASH_KEY(GetRandomHorseHash().c_str());
    Hash skinnerHash1 = MISC::GET_HASH_KEY(GetRandomSkinnerHash().c_str());
    Hash skinnerHash2 = MISC::GET_HASH_KEY(GetRandomSkinnerHash().c_str());

    Vector3 spawnPosition1 = getRandomPedPositionInRange(playerPos, 80);
    Vector3 spawnPosition2 = getRandomPedPositionInRange(playerPos, 80);
    Ped horse1 = QuickDrawImmersion::createPed(horseHash1, spawnPosition1);
    Ped horse2 = QuickDrawImmersion::createPed(horseHash2, spawnPosition2);
    logMessage("Horses created.");
    
    WAIT(600);
    QuickDrawImmersion::giveComboSaddleToHorse(horse1);
    QuickDrawImmersion::giveComboSaddleToHorse(horse2);
    
    Ped ped1 = QuickDrawImmersion::pedMount(skinnerHash1, horse1, true, playerPos, 80);
    Ped ped2 = QuickDrawImmersion::pedMount(skinnerHash2, horse2, true, playerPos, 80);
    logMessage("Peds mounted on horses.");
    logMessage("Peds mounted on horses.");

    WAIT(4500);

    skinnerGroup(ped1, ped2);
    personalityPed(ped1);
    personalityPed(ped2);
    ConfigurePedInteraction(ped1);
    ConfigurePedInteraction(ped2);
    logMessage("Peds personality set and interactions configured.");

    if (ENTITY::DOES_ENTITY_EXIST(ped1) && !ENTITY::IS_ENTITY_DEAD(ped1) &&
        ENTITY::DOES_ENTITY_EXIST(ped2) && !ENTITY::IS_ENTITY_DEAD(ped2)) {

        if (PATHFIND::IS_POINT_ON_ROAD(spawnCoords.x, spawnCoords.y, spawnCoords.z, 0)) {
            logMessage("Point is navigable, moving Ped 1.");
            TASK::TASK_MOVE_FOLLOW_ROAD_USING_NAVMESH(ped1, 1.25f, spawnCoords.x, spawnCoords.y, spawnCoords.z, 0);
            ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(ped1, true);

            constexpr float SAFE_DISTANCE = 2.0f;
            constexpr float MIN_FOLLOW_DISTANCE = 1.5f;

            float followDistance = CalculateDistance1(ENTITY::GET_ENTITY_COORDS(ped1, true, false),
                                                     ENTITY::GET_ENTITY_COORDS(ped2, true, false));

            logMessage("Ped 1 and Ped 2 distance calculated.");

            if (followDistance > MIN_FOLLOW_DISTANCE && !ENTITY::IS_ENTITY_DEAD(ped2)) {
                logMessage("Ped 2 following Ped 1.");
                TASK::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(ped2, ped1, 0.0f, -SAFE_DISTANCE, 0.0f, 1.25f, -1, 0.5f, true, false, false, false, false, false);
                ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(ped2, true);
            } else {
                logMessage("Ped 2 is too close to Ped 1, standing still to adjust.");
                TASK::TASK_STAND_STILL(ped2, 500);
            }
        } else {
            logMessage("NavMesh path query failed for Ped 1. Deleting entities and retrying.");
            ENTITY::DELETE_ENTITY(&ped1);
            ENTITY::DELETE_ENTITY(&ped2);
            ENTITY::DELETE_ENTITY(&horse1);
            ENTITY::DELETE_ENTITY(&horse2);
            SkinnerMountSpawn();
            return;
        }
    }

    globalEntityList.push_back(ped1);
    globalEntityList.push_back(ped2);
    vehicleEntityList.push_back(horse1);
    vehicleEntityList.push_back(horse2);
}
