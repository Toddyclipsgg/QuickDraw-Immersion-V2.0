#include "..\header.h"

// Variáveis globais
std::vector<Ped> pedList; // Lista global de peds
std::map<Ped, Blip> pedBlips; // Mapa para armazenar a relação ped -> blip

Blip createBlip(Vector3 pos, Hash blipType, Hash blipSprite)
{
	Blip blip;
	Object dummyProp = createProp("p_shotGlass01x", pos, true, false, false);

	blip = MAP::BLIP_ADD_FOR_ENTITY(blipType, dummyProp); // Add blip for dummy prop
	if (blipSprite != 0)
	{
		MAP::SET_BLIP_SPRITE(blip, blipSprite, false);
	}

	return blip;
}

Blip createBlip(Entity entity, Hash blipType, Hash blipSprite)
{
	Blip blip = MAP::BLIP_ADD_FOR_ENTITY(blipType, entity);

	if (blipSprite != 0)
	{
		MAP::SET_BLIP_SPRITE(blip, blipSprite, false);
	}

	return blip;
}

Blip createBlip(Vector3 source, float radius, Hash blipType, Hash blipSprite)
{
	Blip blip = MAP::BLIP_ADD_FOR_RADIUS(blipType, source.x, source.y, source.z, radius); // add blip for area

	if (blipSprite != 0)
	{
		MAP::SET_BLIP_SPRITE(blip, blipSprite, false);
	}

	return blip;
}

void setBlipLabel(Blip blip, const char* label)
{
	MAP::_SET_BLIP_NAME(blip, MISC::VAR_STRING(10, "LITERAL_STRING", label)); // _SET_BLIP_NAME_FROM_PLAYER_STRING
}

void deleteBlipSafe(Blip* blip)
{
	if (MAP::DOES_BLIP_EXIST(*blip))
	{
		MAP::REMOVE_BLIP(blip);
	}
}

Object createProp(const char* model, Vector3 position, float heading, bool isStatic, bool isVisible)
{
	Hash modelHash = MISC::GET_HASH_KEY(model);

	if (!STREAMING::HAS_MODEL_LOADED(modelHash))
	{
		STREAMING::REQUEST_MODEL(modelHash, false);
	}

	while (!STREAMING::HAS_MODEL_LOADED(modelHash))
	{
		WAIT(0);
	}

	Object prop = OBJECT::CREATE_OBJECT(modelHash, position.x, position.y, position.z, false, false, !isStatic, 0, 0);
	ENTITY::SET_ENTITY_HEADING(prop, heading);
	ENTITY::FREEZE_ENTITY_POSITION(prop, isStatic);
	ENTITY::SET_ENTITY_VISIBLE(prop, isVisible);

	return prop;
}

// Função para gerenciar o blip de um ped
void ManagePedBlip(Ped ped, Player player) {
	// Verificar se o ped existe e está vivo
	if (ENTITY::DOES_ENTITY_EXIST(ped) && !ENTITY::IS_ENTITY_DEAD(ped)) {
		Blip& targetBlip = pedBlips[ped]; // Referência para o blip associado ao ped

		// Se o blip ainda não foi criado, criá-lo
		if (targetBlip == NULL) {
			targetBlip = createBlip(ped, MISC::GET_HASH_KEY("BLIP_STYLE_RANDOM_EVENT_PULSE"));
			logMessage("Blip created for the ped.");
		}

		// Verificar linha de visão entre o jogador e o ped para remover o blip
		if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(player, ped, 1)) {
			// Se há linha de visão, remover o blip
			deleteBlipSafe(&targetBlip);
			logMessage("Blip removed due to clear LOS.");
		}
	}
	else {
		// Se o ped estiver morto ou não existir, remover o blip
		Blip& targetBlip = pedBlips[ped];
		if (targetBlip != NULL) {
			deleteBlipSafe(&targetBlip);
			targetBlip = NULL;  // Limpar o blip no mapa
			logMessage("Blip removed because ped is dead or doesn't exist.");
		}
	}
}

// Função no loop principal para gerenciar os blips de todos os peds
void ManageAllPedBlips(Player player) {
	for (auto& ped : pedList) {
		ManagePedBlip(ped, player); // Chamar a função para cada ped
	}
}
