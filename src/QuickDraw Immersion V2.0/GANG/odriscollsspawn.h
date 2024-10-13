#pragma once

static std::vector<std::string> odriscoll = {
	"G_M_M_UNIDUSTER_03",
	"G_M_M_UNIDUSTER_02",
	"G_M_M_UNIDUSTER_01",
	"U_M_M_UniDusterLeader_01",
	"G_M_M_UniDuster_01",
	"G_M_M_UniDuster_02",
	"G_M_M_UniDuster_03",
	"G_M_M_UniDuster_04",

};

/*
static std::vector<std::string> lemoyne = {
	"G_M_Y_UNIEXCONFEDS_02",
	"G_M_Y_UNIEXCONFEDS_01",
	"G_M_O_UNIEXCONFEDS_01",
	"CS_EXCONFEDSLEADER_01"
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
std::string GetRandomOdriscollHash();
