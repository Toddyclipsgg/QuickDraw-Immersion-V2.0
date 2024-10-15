#pragma once

static std::vector<std::string> lemoyne = {
	"G_M_O_UniExConfeds_01",
	"G_M_Y_UniExConfeds_01",
	"G_M_Y_UNIEXCONFEDS_02"
};

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

void LemoyneMountSpawn();
std::string GetRandomLemoyneHash();
