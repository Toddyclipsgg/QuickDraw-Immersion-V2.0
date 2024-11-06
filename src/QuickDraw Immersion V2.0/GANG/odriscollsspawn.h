#pragma once

static std::vector<std::string> odriscoll = {
	"G_M_M_UNIDUSTER_03",
	"G_M_M_UNIDUSTER_02",
	"G_M_M_UNIDUSTER_01",
	"G_M_M_UniDuster_01",
	"G_M_M_UniDuster_02",
	"G_M_M_UniDuster_03",
	"G_M_M_UniDuster_04",

};

/*
static std::vector<std::string> lemoyne = {
	"G_M_O_UniExConfeds_01",
"G_M_Y_UniExConfeds_01",
"G_M_Y_UNIEXCONFEDS_02"
};
*/

/*
static std::vector<std::string> murfrees = {
	"G_M_M_UNIINBRED_01"
};
*/

/*
static std::vector<std::string> skinners = {
	"U_M_M_BHT_SKINNERSEARCH",
	"G_M_M_UNIMOUNTAINMEN_01",
	"U_M_M_BHT_SKINNERBROTHER"
};
*/

/*
static std::vector<std::string> bronte = {
	"U_M_M_NBXBRONTEGOON_01",
	"U_M_M_NBXBRONTESECFORM_01",
	"U_M_M_NBXBRONTEASC_01",
	"G_M_M_UNIBRONTEGOONS_01"
};
*/

// PED::SET_PED_CONFIG_FLAG(ped, 130 /*PCF_DisableTalkTo*/, false);  // Permite interação de fala
// PED::SET_PED_CONFIG_FLAG(ped, 297 /*PCF_ForceInteractionLockonOnTargetPed*/, true);  // Força lock-on de interação
// PED::SET_PED_CONFIG_FLAG(ped, 301 /*PCF_DisableInteractionLockonOnTargetPed*/, false);  // Permite lock-on
// PED::SET_PED_CONFIG_FLAG(ped, 174 /*PCF_DisableEvasiveStep*/, true);
// PED::SET_PED_CONFIG_FLAG(ped, 154 /*PCF_UseFollowLeaderThreatResponse*/, true);
// PED::SET_PED_CONFIG_FLAG(ped, 89 /*PCF_DisablePanicInVehicle*/, true);
// PED::SET_PED_CONFIG_FLAG(ped, 6 /*PCF_DontInfluenceWantedLevel*/, true);
// PED::SET_PED_CONFIG_FLAG(ped, 233 /*PCF_PedIsEnemyToPlayer*/, true);
// PED::SET_PED_CONFIG_FLAG(ped, 167 /*PCF_ShouldBeOnMount*/, true);
// PED::SET_PED_CONFIG_FLAG(ped, 307 /*PCF_DisableMountSpawning*/, true);
// PED::SET_PED_CONFIG_FLAG(ped, 361 /*PCF_IgnoreWeaponDegradation*/, true);
// PED::SET_PED_CONFIG_FLAG(ped, 279 /*PCF_NeverLeavesGroup*/, true);

void OdriscollsMountSpawn();
void generateHorsesAndPeds(Vector3 playerPos, Ped& horse1, Ped& horse2, Ped& ped1, Ped& ped2);
void setupPeds(Ped ped1, Ped ped2);
bool areEntitiesValid(Ped ped1, Ped ped2);
std::pair<std::string, Vector3> findValidDestination(Vector3 pedCoords);
void movePedsToDestination(Ped ped1, Ped ped2, Vector3 destino);
void deleteEntities(Ped& ped1, Ped& ped2, Ped& horse1, Ped& horse2);
std::string GetRandomOdriscollHash();
void OdriscollsVehicleSpawn();
bool getRoadPositionAndHeading(const Vector3& playerPos, float spawnDistance, Vector3& roadPos, float& roadHeading);
bool findValidDestination(Vector3& destino, float minDistance, float maxDistance, int maxAttempts);
Entity createVehicleWithCargo(const Vector3& roadPos, float roadHeading);
void configurePedsForVehicle(Ped ped1, Ped ped2, Entity vehicle, Player player);
bool isValidVehicleAndDestination(Ped ped1, Entity vehicle, const Vector3& destino);
void moveVehicleToDestination(Ped ped1, Entity vehicle, const Vector3& destino);
void deleteEntities(Ped& ped1, Ped& ped2, Entity& vehicle);
