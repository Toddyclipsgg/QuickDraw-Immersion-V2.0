#include "..\header.h"

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

// Fun��o para gerenciar o blip do ped (cria��o e remo��o)
void ManagePedBlip(Ped ped, Player player) {
	// Vari�vel do blip criada aqui para ser acess�vel em ambos os blocos
	static Blip targetBlip = NULL;
	static bool seen = false;  // Vari�vel de controle para garantir que o blip seja atualizado corretamente

	// Verificar se o ped existe e est� vivo antes de criar o blip
	if (ENTITY::DOES_ENTITY_EXIST(ped) && !ENTITY::IS_ENTITY_DEAD(ped)) {
		// Se o blip ainda n�o foi criado, cri�-lo
		if (targetBlip == NULL) {
			targetBlip = createBlip(ped, MISC::GET_HASH_KEY("BLIP_STYLE_EVENT_AREA"));
			logMessage("Blip created for the ped.");
		}

		// Verificar se o jogador e o ped t�m linha de vis�o clara um do outro para remover o blip
		if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(player, ped, 1) &&
			ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(ped, player, 1) && !seen) {

			// Remover o blip com seguran�a
			deleteBlipSafe(&targetBlip);
			logMessage("Blip removed.");
			seen = true; // Atualiza a vari�vel de controle
		}
		else if (!ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(player, ped, 1) ||
			!ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(ped, player, 1)) {
			// Se a linha de vis�o n�o estiver clara, redefinir seen
			seen = false; // Permitir que o blip seja recriado
		}
	}
	else {
		// Caso o ped esteja morto ou n�o exista, remover o blip
		if (targetBlip != NULL) {
			deleteBlipSafe(&targetBlip);
			targetBlip = NULL;  // Redefinir o blip ap�s a exclus�o
			logMessage("Blip removed because ped does not exist or is dead.");
		}
		seen = false;  // Resetar a vari�vel de controle
	}
}

// createBlip(ped, MISC::GET_HASH_KEY("BLIP_STYLE_EVENT_AREA")
