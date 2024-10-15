int ConfigurarInteracaoComPed(Ped ped, float distanciaInteracao, bool ativarLockon, bool registrarPrompt) {
    // Primeira etapa: desregistrar o prompt de interação existente, se aplicável
    if (ativarLockon) {
        MISC::UNREGISTER_INTERACTION_LOCKON_PROMPT(ped);
    }

    // Garantir que o Ped permite eventos não temporários (necessário para interações)
    if (!PED::GET_PED_CONFIG_FLAG(ped, 178 /*PCF_AllowNonTempExceptionEvents*/, true)) {
        PED::SET_PED_CONFIG_FLAG(ped, 178 /*PCF_AllowNonTempExceptionEvents*/, true);
    }

    // Força a ativação ou desativação da flag de lock-on de interação
    if (PED::GET_PED_CONFIG_FLAG(ped, 297 /*PCF_ForceInteractionLockonOnTargetPed*/, true) != ativarLockon) {
        PED::SET_PED_CONFIG_FLAG(ped, 297 /*PCF_ForceInteractionLockonOnTargetPed*/, ativarLockon);
    }

    // Desativa restrições que impedem interações
    if (PED::GET_PED_CONFIG_FLAG(ped, 315 /*PCF_0xD38AEF95*/, true)) {
        PED::SET_PED_CONFIG_FLAG(ped, 315 /*PCF_0xD38AEF95*/, false);
    }
    if (PED::GET_PED_CONFIG_FLAG(ped, 331 /*PCF_0x9663C8F2*/, true)) {
        PED::SET_PED_CONFIG_FLAG(ped, 331 /*PCF_0x9663C8F2*/, false);
    }
    if (PED::GET_PED_CONFIG_FLAG(ped, 130 /*PCF_DisableTalkTo*/, true)) {
        PED::SET_PED_CONFIG_FLAG(ped, 130 /*PCF_DisableTalkTo*/, false);
    }
    if (PED::GET_PED_CONFIG_FLAG(ped, 301 /*PCF_DisableInteractionLockonOnTargetPed*/, true)) {
        PED::SET_PED_CONFIG_FLAG(ped, 301 /*PCF_DisableInteractionLockonOnTargetPed*/, false);
    }

    // Caso o lock-on e o prompt devam ser ativados, registre o novo prompt
    if (ativarLockon && registrarPrompt) {
        if (MISC::REGISTER_INTERACTION_LOCKON_PROMPT(ped, "INTERACT_LOCKON" /* GXTEntry: "Interact" */, distanciaInteracao, 0.0f, 0, 0.0f, 0.0f, 0, false, -1)) {
            logMessage("Prompt de interação registrado com sucesso.");
            return 1; // Sucesso
        }
    }
    else {
        return 1; // Sucesso, mesmo sem registrar prompt
    }

    return 0; // Falha ao registrar prompt
}


void ConfigurarInteracaoPed(Ped ped) {
    // Primeiro, garantimos que todos os NPCs ignorem o jogador temporariamente
    logMessage("Desativando interações com todos os NPCs para configurar.");

    // PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_PED_ID(), true); // Todos os NPCs ignoram o jogador temporariamente

    if (!ENTITY::IS_ENTITY_DEAD(ped)) {
        logMessage("Configurando personalidade e comportamento do Ped.");

        // Configura personalidade e comportamento de interação
        PED::_SET_PED_INTERACTION_PERSONALITY(ped, MISC::GET_HASH_KEY("SCRIPTEDINTIMIDATION"));  // Personalidade para permitir interações específicas
        PED::_SET_PED_PERSONALITY(ped, MISC::GET_HASH_KEY("STANDARD_PED_AGRO"));  // Personalidade padrão para interações não agressivas

        // Ativa as configurações de flags que permitem interação com o Ped
        PED::SET_PED_CONFIG_FLAG(ped, 130 /*PCF_DisableTalkTo*/, false);  // Permite interação de fala
        PED::SET_PED_CONFIG_FLAG(ped, 297 /*PCF_ForceInteractionLockonOnTargetPed*/, true);  // Força lock-on de interação
        PED::SET_PED_CONFIG_FLAG(ped, 301 /*PCF_DisableInteractionLockonOnTargetPed*/, false);  // Permite lock-on de interação
        PED::SET_PED_CONFIG_FLAG(ped, 431 /*PCF_0xA5CB7C09*/, true);  // Permite interações mesmo em combate
        PED::SET_PED_CONFIG_FLAG(ped, 452 /*PCF_0x9909D028*/, true);  // Configuração adicional para interações

        // Configura o Ped como alvo de prioridade para interações
        ENTITY::SET_ENTITY_IS_TARGET_PRIORITY(ped, true, 0.0f);

        // Ativa os prompts de saudação e antagonizar na UI do jogador
        PLAYER::_MODIFY_PLAYER_UI_PROMPT_FOR_PED(PLAYER::PLAYER_PED_ID(), ped, 19 /*Greet*/, 2, true);  // Ativa saudação
        PLAYER::_MODIFY_PLAYER_UI_PROMPT_FOR_PED(PLAYER::PLAYER_PED_ID(), ped, 20 /*Antagonize*/, 2, true);  // Ativa antagonizar

        // Força a atualização das animações e comportamento do Ped
        PED::FORCE_PED_AI_AND_ANIMATION_UPDATE(ped, false, false);
        logMessage("Configuração de interações concluída para o Ped.");
    }
    else {
        logMessage("O Ped está morto. Não foi possível configurar.");
    }

    // Após configurar, restauramos as interações normais com todos os NPCs
    logMessage("Restaurando interações com todos os NPCs.");
    // PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_PED_ID(), false);  // Restabelece as interações para outros NPCs
}


AUDIO::SET_AMBIENT_VOICE_NAME(Ped ped, const char* name);

char[] func_177(int iParam0)
{
	if (func_359(iParam0))
	{
		switch (iParam0)
		{
		case 0:
			return "GREET_POS" /* GXTEntry: "Accept" */;
		case 1:
			return "GREET_NEG" /* GXTEntry: "Decline" */;
		case 2:
			return "GREET_SPOS" /* GXTEntry: "Positive" */;
		case 3:
			return "GREET_SNEG" /* GXTEntry: "Negative" */;
		case 4:
			return "GREET_ACC" /* GXTEntry: "Accept" */;
		case 5:
			return "GREET_REJ" /* GXTEntry: "Reject" */;
		case 6:
			return "BEAT_GRT_NEG" /* GXTEntry: "Decline" */;
		case 7:
			return "INTERACT_GREET" /* GXTEntry: "Greet" */;
		case 8:
			return "INTERACT_DEFUSE" /* GXTEntry: "Defuse" */;
		case 9:
			return "INTERACT_LET_GO" /* GXTEntry: "Dismiss" */;
		case 10:
			return "INTERACT_INSULT" /* GXTEntry: "Antagonize" */;
		case 11:
			return "INTERACT_THREATEN" /* GXTEntry: "Threaten" */;
		case 12:
			return "INTERACT_ROB" /* GXTEntry: "Rob" */;
		case 13:
			return "INTERACT_ROB_GRAPPLE_LET_GO" /* GXTEntry: "Dismiss" */;
		case 14:
			return "INTERACT_ROB_ACCEPT" /* GXTEntry: "Dismiss" */;
		case 15:
			return "INTERACT_ROB_REJECT_MELEE" /* GXTEntry: "Beat" */;
		case 16:
			return "INTERACT_STOP_WITNESS" /* GXTEntry: "Stop Witness" */;
		case 17:
			return "INTERACT_STOP_VEHICLE" /* GXTEntry: "Callout" */;
		case 18:
			return "INTERACT_STOP_HORSE" /* GXTEntry: "Callout" */;
		case 19:
			return "INTERACT_REQUEST_RIDE" /* GXTEntry: "Request Ride" */;
		case 20:
			return "INTERACT_STEAL_HORSE" /* GXTEntry: "Steal ~1~" */;
		case 21:
			return "INTERACT_STEAL_VEHICLE" /* GXTEntry: "Steal Vehicle" */;
		case 22:
			return "INTERACT_HORSE" /* GXTEntry: "Pat" */;
		case 23:
			return "INTERACT_CALLOUT" /* GXTEntry: "Call Out" */;
		case 24:
			return "INTERACT_QUESTION" /* GXTEntry: "Question" */;
		case 25:
			return "INTERACT_ENCOURAGE" /* GXTEntry: "Encourage" */;
		case 26:
			return "INTERACT_INTERVENE" /* GXTEntry: "Intervene" */;
		case 27:
			return "INTERACT_CONFRONT" /* GXTEntry: "Confront" */;
		case 28:
			return "RE_INTER_TRUTH" /* GXTEntry: "Truth" */;
		case 29:
			return "RE_INTER_LIE" /* GXTEntry: "Lie" */;
		case 30:
			return "INTERACT_QUIT" /* GXTEntry: "Quit" */;
		case 31:
			return "RE_INTER_DIRECT" /* GXTEntry: "Direction" */;
		case 32:
			return "INTERACT_SCOLD" /* GXTEntry: "Scold" */;
		case 33:
			return "INTERACT_PRAISE" /* GXTEntry: "Praise" */;
		case 34:
			return "INTERACT_ASSIST" /* GXTEntry: "Assist" */;
		case 35:
			return "INTERACT_MISLEAD" /* GXTEntry: "Mislead" */;
		case 36:
			return "INTERACT_ASK" /* GXTEntry: "Ask" */;
		case 37:
			return "INTERACT_DEMAND" /* GXTEntry: "Demand" */;
		default:
			break;
		}
	}
	return "PED_INTERACT_PROMPT_LABEL_ERROR";
}

void ConfigurarInteracaoPed(Ped ped) {

    if (!ENTITY::IS_ENTITY_DEAD(ped) && !PED::IS_PED_DEAD_OR_DYING(ped, true)) {
        logMessage("Configurando personalidade e comportamento do Ped.");

        // Configura personalidade e comportamento de interação
        // PED::_SET_PED_INTERACTION_PERSONALITY(ped, MISC::GET_HASH_KEY("SCRIPTEDINTIMIDATION"));  // Personalidade de intimidação
        // PED::_SET_PED_PERSONALITY(ped, MISC::GET_HASH_KEY("AGGRESSIVE"));  // Personalidade padrão agressiva

        // Define o nome da voz que o Ped usará
        //AUDIO::SET_AMBIENT_VOICE_NAME(ped, "0361_G_M_M_UniDuster_03_WHITE_01");  // Definindo áudio para saudação positiva (GREET_POS)
        // PED::_SET_PED_INTERACTION_POSITIVE_RESPONSE(ped, "");
        // PED::_SET_PED_INTERACTION_NEGATIVE_RESPONSE(ped, "");

        // Configura as flags de interação e fala
        PED::SET_PED_CONFIG_FLAG(ped, 130 /*PCF_DisableTalkTo*/, false);  // Permite interação de fala
        PED::SET_PED_CONFIG_FLAG(ped, 297 /*PCF_ForceInteractionLockonOnTargetPed*/, true);  // Força lock-on de interação
        PED::SET_PED_CONFIG_FLAG(ped, 301 /*PCF_DisableInteractionLockonOnTargetPed*/, false);  // Permite lock-on
        // PED::SET_PED_CONFIG_FLAG(ped, 431 /*PCF_0xA5CB7C09*/, true);  // Permite interação durante combate
        // PED::SET_PED_CONFIG_FLAG(ped, 178 /*PCF_AllowNonTempExceptionEvents*/, true);  // Habilita eventos não temporários
        // PED::SET_PED_CONFIG_FLAG(ped, 225 /*PCF_0x0C5FB46A*/, true);  // Configuração extra para respostas

        // Configura o Ped como alvo prioritário para interações
        // ENTITY::SET_ENTITY_IS_TARGET_PRIORITY(ped, true, 0.0f);

        // Ativa os prompts de saudação e antagonizar na UI do jogador
        // PLAYER::_MODIFY_PLAYER_UI_PROMPT_FOR_PED(PLAYER::PLAYER_PED_ID(), ped, 19 /*Greet*/, 2, true);  // Ativa saudação
        // PLAYER::_MODIFY_PLAYER_UI_PROMPT_FOR_PED(PLAYER::PLAYER_PED_ID(), ped, 20 /*Antagonize*/, 2, true);  // Ativa antagonizar

        // Controle de áudio e resposta de fala
        if (!PED::IS_PED_DEAD_OR_DYING(ped, true)) {
            logMessage("Preparando Ped para responder.");

            // Faz o Ped olhar para o jogador
            // TASK::TASK_LOOK_AT_ENTITY(ped, PLAYER::PLAYER_PED_ID(), 7500, 0, 51, 0);
            // AUDIO::STOP_CURRENT_PLAYING_AMBIENT_SPEECH(ped, 0);  // Interrompe qualquer fala anterior
            // AUDIO::STOP_CURRENT_PLAYING_SPEECH(ped, 0);  // Garante que o diálogo não está sendo sobreposto

            // Configura flags de reset para permitir nova fala
            // PED::SET_PED_RESET_FLAG(ped, 184 /*PRF_DisableAutoGreets*/, true);  // Desativa saudações automáticas temporariamente
            // PED::SET_PED_RESET_FLAG(ped, 281 /*PRF_0xC5C24250*/, true);  // Força o Ped a processar um novo evento de fala

            // Ativa o diálogo de saudação usando o comando de áudio
            // AUDIO::_0x72E4D1C4639BC465(ped, 0);  // Inicia o diálogo de interação (usando o ID correto para sua implementação)

            logMessage("Ped configurado para responder ao jogador com fala.");
        }
        else {
            logMessage("Ped não pode responder porque está morto ou morrendo.");
        }
    }
    else {
        logMessage("O Ped está morto. Não foi possível configurar.");
    }
}

void func_353(int iParam0, int iParam1, float fParam2)
{
	bool bVar0;
	int iVar1;

	bVar0 = true;
	iVar1 = ENTITY::GET_ENTITY_MODEL(iParam0);
	if (((((((*iParam1 == joaat("WEAPON_REVOLVER_CATTLEMAN_MEXICAN") || *iParam1 == joaat("WEAPON_REVOLVER_CATTLEMAN_PIG")) || *iParam1 == joaat("WEAPON_REVOLVER_DOUBLEACTION_EXOTIC")) || *iParam1 == joaat("WEAPON_REVOLVER_SCHOFIELD_CALLOWAY")) || *iParam1 == joaat("WEAPON_REVOLVER_SCHOFIELD_GOLDEN")) || *iParam1 == joaat("WEAPON_PISTOL_MAUSER_DRUNK")) || *iParam1 == joaat("WEAPON_SHOTGUN_DOUBLEBARREL_EXOTIC")) || *iParam1 == joaat("WEAPON_SNIPERRIFLE_ROLLINGBLOCK_EXOTIC"))
	{
		*fParam2 = 0.0f;
		return;
	}
	switch (iVar1)
	{
	case joaat("G_M_M_UNIDUSTER_03"):
	case joaat("U_M_M_UNIDUSTERHENCHMAN_03"):
	case joaat("G_M_M_UNIDUSTER_04"):
	case joaat("U_M_M_UNIDUSTERHENCHMAN_02"):
	case joaat("G_M_M_UNIDUSTER_02"):
	case joaat("U_M_M_FATDUSTER_01"):
	case joaat("U_M_M_UNIDUSTERHENCHMAN_01"):
	case joaat("G_M_M_UNIDUSTER_01"):
	case joaat("CS_DISGUISEDDUSTER_01"):
	case joaat("G_F_M_UNIDUSTER_01"):
	case joaat("U_M_M_UNIDUSTERLEADER_01"):
	case joaat("CS_DISGUISEDDUSTER_03"):
	case joaat("G_M_M_UNIDUSTER_05"):
	case joaat("CS_DISGUISEDDUSTER_02"):
		*fParam2 = MISC::GET_RANDOM_FLOAT_IN_RANGE(0.3f, 0.5f);
		break;
	case joaat("G_M_Y_UNIEXCONFEDS_02"):
	case joaat("G_M_Y_UNIEXCONFEDS_01"):
	case joaat("G_M_O_UNIEXCONFEDS_01"):
	case joaat("CS_EXCONFEDSLEADER_01"):
	case joaat("U_M_M_UNIEXCONFEDSBOUNTY_01"):
		*fParam2 = MISC::GET_RANDOM_FLOAT_IN_RANGE(0.125f, 0.375f);
		break;
	case joaat("G_M_M_UNIINBRED_01"):
		*fParam2 = MISC::GET_RANDOM_FLOAT_IN_RANGE(0.7f, 0.875f);
		break;
	case joaat("U_M_M_BHT_SKINNERSEARCH"):
	case joaat("G_M_M_UNIMOUNTAINMEN_01"):
	case joaat("U_M_M_BHT_SKINNERBROTHER"):
		*fParam2 = MISC::GET_RANDOM_FLOAT_IN_RANGE(0.4f, 0.6f);
		break;
	case joaat("G_M_M_UNIRANCHERS_01"):
	case joaat("CS_LARAMIE"):
		*fParam2 = MISC::GET_RANDOM_FLOAT_IN_RANGE(0.125f, 0.25f);
		break;
	case joaat("U_M_M_BHT_BANDITOSHACK"):
	case joaat("U_M_M_BHT_BANDITOMINE"):
	case joaat("G_M_M_UNIBANDITOS_01"):
		*fParam2 = MISC::GET_RANDOM_FLOAT_IN_RANGE(0.3f, 0.5f);
		break;
	case joaat("U_M_M_NBXBRONTEGOON_01"):
	case joaat("U_M_M_NBXBRONTESECFORM_01"):
	case joaat("U_M_M_NBXBRONTEASC_01"):
	case joaat("G_M_M_UNIBRONTEGOONS_01"):
		*fParam2 = MISC::GET_RANDOM_FLOAT_IN_RANGE(0.125f, 0.125f);
		break;
	default:
		*fParam2 = MISC::GET_RANDOM_FLOAT_IN_RANGE(0.3f, 0.4f);
		if (PED::GET_PED_RELATIONSHIP_GROUP_HASH(iParam0) == joaat("REL_GANG_DUTCHS"))
		{
			bVar0 = false;
		}
		break;
	}
	if (bVar0)
	{
		PED::SET_PED_CONFIG_FLAG(iParam0, 361 /*PCF_IgnoreWeaponDegradation*/, true);
	}
}

std::vector<Ped>& ped1List;
std::vector<Ped>& ped2List;

// Função para monitorar o movimento dos Peds
bool IsPedStopped(Ped ped, Vector3* lastCoords, float threshold) {
    Vector3 currentCoords = ENTITY::GET_ENTITY_COORDS(ped, true, false);
    float distance = BUILTIN::VDIST(currentCoords.x, currentCoords.y, currentCoords.z, lastCoords->x, lastCoords->y, lastCoords->z);

    // Se a distância percorrida for menor que o threshold, considera-se que o ped está parado
    if (distance < threshold) {
        return true;
    }

    // Atualiza as últimas coordenadas
    *lastCoords = currentCoords;
    return false;
}

// Função para calcular distância 3D entre o ped e as coordenadas fornecidas
float CalcularDistancia(Vector3 pedCoords, Vector3 destinoCoords) {
    return BUILTIN::VDIST(pedCoords.x, pedCoords.y, pedCoords.z, destinoCoords.x, destinoCoords.y, destinoCoords.z);
}

void monitoringPeds(std::vector<Ped>& ped) {

    // Monitoramento de movimento
    Vector3 lastCoords1 = ENTITY::GET_ENTITY_COORDS(ped1, true, false);
    Vector3 lastCoords2 = ENTITY::GET_ENTITY_COORDS(ped2, true, false);
    int timeNotMoving1 = 0;
    int timeNotMoving2 = 0;

    // Loop de monitoramento
    while (true) {
        WAIT(1000);  // Checar a cada segundo

        // Verifica se ped1 está parado
        if (IsPedStopped(ped1, &lastCoords1, 0.5f)) {
            timeNotMoving1 += 1;
        }
        else {
            timeNotMoving1 = 0;  // Reseta o contador se ele estiver se movendo
        }

        // Verifica se ped2 está parado
        if (IsPedStopped(ped2, &lastCoords2, 0.5f)) {
            timeNotMoving2 += 1;
        }
        else {
            timeNotMoving2 = 0;  // Reseta o contador se ele estiver se movendo
        }

        // Exclui os peds se estiverem parados por mais de 5 segundos
        if (timeNotMoving1 >= 5) {
            logMessage("Ped1 está parado por muito tempo, excluindo.");
            ENTITY::DELETE_ENTITY(&ped1);
            break;
        }

        if (timeNotMoving2 >= 5) {
            logMessage("Ped2 está parado por muito tempo, excluindo.");
            ENTITY::DELETE_ENTITY(&ped2);
            break;
        }
    }
}

void distancPed() {

    // Configurar offset e garantir que ped2 siga sem ultrapassar
    float safeDistance = 2.0f;  // Distância segura atrás de ped1
    float minDistance = 1.5f;   // Distância mínima para seguir

    Vector3 ped1Position = ENTITY::GET_ENTITY_COORDS(ped1, true, false);
    Vector3 ped2Position = ENTITY::GET_ENTITY_COORDS(ped2, true, false);

    // Calcula a distância entre ped1 e ped2
    float distance = BUILTIN::VDIST(ped1Position.x, ped1Position.y, ped1Position.z, ped2Position.x, ped2Position.y, ped2Position.z);

    // Certifique-se de que ped2 sempre siga atrás de ped1, mantendo uma distância segura
    if (distance > minDistance) {
        // Se ped2 estiver longe, segue ped1
        TASK::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(ped2, ped1, 0.0f, -safeDistance, 0.0f, 1.001f, -1, 0.500f, true, false, false, false, false, false);
        logMessage("Ped2 está seguindo ped1 a uma distância segura.");
    }
    else {
        // Se ped2 estiver muito perto, desacelere para evitar bloqueio
        TASK::TASK_STAND_STILL(ped2, 500); // Espera 500ms para manter distância
        logMessage("Ped2 está muito próximo, ajustando para evitar bloqueio.");
    }
}

/*
// Função para mover dois peds montados para uma coordenada específica
void MoveMountedPedsToCoords(float speed, bool enableInteraction, bool aggressivePersonality) {
    // ID do jogador
    Ped playerPed = PLAYER::PLAYER_PED_ID();
    logMessage("ID do Ped do jogador obtido.");

    // Pega as coordenadas atuais do jogador
    Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);
    logMessage("Coordenadas do jogador obtidas: " + std::to_string(playerPos.x) + ", " + std::to_string(playerPos.y) + ", " + std::to_string(playerPos.z));

    // Gerar hash para os cavalos aleatórios
    std::string randomHorse1 = GetRandomHorseHash();
    Hash horseHash1 = MISC::GET_HASH_KEY(randomHorse1.c_str());
    logMessage("Hash do cavalo 1: " + randomHorse1);

    std::string randomHorse2 = GetRandomHorseHash();
    Hash horseHash2 = MISC::GET_HASH_KEY(randomHorse2.c_str());
    logMessage("Hash do cavalo 2: " + randomHorse2);

    // Gerar hash para os O'Driscoll aleatórios
    std::string randomOdriscoll1 = GetRandomOdriscollHash();
    Hash odriscollHash1 = MISC::GET_HASH_KEY(randomOdriscoll1.c_str());
    logMessage("Hash do O'Driscoll 1: " + randomOdriscoll1);

    std::string randomOdriscoll2 = GetRandomOdriscollHash();
    Hash odriscollHash2 = MISC::GET_HASH_KEY(randomOdriscoll2.c_str());
    logMessage("Hash do O'Driscoll 2: " + randomOdriscoll2);

    // Criando os cavalos aleatórios
    Ped horse1 = createPed(horseHash1, getRandomPedPositionInRange(playerPos, 60));
    Ped horse2 = createPed(horseHash2, getRandomPedPositionInRange(playerPos, 60));

    // Atribuir sela aos cavalos
    giveSaddleToHorse(horse1, MCCLELLAN_01_STOCK_NEW_SADDLE_004);
    giveSaddleToHorse(horse2, MCCLELLAN_01_STOCK_NEW_SADDLE_004);
    logMessage("Selas aplicadas aos cavalos.");

    // Criar os peds montados em seus respectivos cavalos
    Ped ped1 = pedMount(odriscollHash1, horse1, true, playerPos, 60);
    Ped ped2 = pedMount(odriscollHash2, horse2, true, playerPos, 60);
    logMessage("Peds montados criados com sucesso.");
    WAIT(5000);

    // Definir a personalidade agressiva e permitir interação
    ConfigurePedInteraction(ped1);
    ConfigurePedInteraction(ped2);
    WAIT(500);

    // Verifica se ambos os peds (ped e cavalo) existem e estão vivos
    if (ENTITY::DOES_ENTITY_EXIST(ped1) && !ENTITY::IS_ENTITY_DEAD(ped1) &&
        ENTITY::DOES_ENTITY_EXIST(ped2) && !ENTITY::IS_ENTITY_DEAD(ped2)) {

        Vector3 ped1Coords = ENTITY::GET_ENTITY_COORDS(ped1, true, false);  // Pega as coordenadas atuais do ped1
        float distanciaMaxima = 1500.0f;  // Defina a distância máxima "perto o suficiente"
        float distanciaMinima = 500.0f;   // Defina a distância mínima "longe o suficiente"
        float distancia = 0.0f;
        int tentativasMax = 25;  // Limite de tentativas para evitar loop infinito
        int tentativaAtual = 0;

        std::pair<std::string, Vector3> destino;

        // Gerar novas coordenadas até encontrar uma suficientemente perto e longe o suficiente
        while (tentativaAtual < tentativasMax) {
            destino = GetRandomCoord();  // Gerar uma coordenada aleatória
            distancia = CalcularDistancia(ped1Coords, destino.second);  // Calcular a distância a partir do ped1

            logMessage("Tentativa " + std::to_string(tentativaAtual) + ": Coordenada gerada - " + destino.first +
                " (" + std::to_string(destino.second.x) + ", " + std::to_string(destino.second.y) + ", " + std::to_string(destino.second.z) +
                "), Distância: " + std::to_string(distancia));

            // Verifica se a distância está entre 500 e 1500 metros
            if (distancia >= distanciaMinima && distancia <= distanciaMaxima) {
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
                // Mover o cavalo diretamente para a coordenada de destino
                TASK::TASK_MOVE_FOLLOW_ROAD_USING_NAVMESH(ped1, 1.001f, x, y, z, 0);
                ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(ped1, true);

                // Configurar offset e garantir que ped2 siga sem ultrapassar
                float safeDistance = 2.0f;  // Distância segura atrás de ped1
                float minDistance = 1.5f;   // Distância mínima para seguir

                Vector3 ped1Position = ENTITY::GET_ENTITY_COORDS(ped1, true, false);
                Vector3 ped2Position = ENTITY::GET_ENTITY_COORDS(ped2, true, false);

                // Calcula a distância entre ped1 e ped2
                float distance = BUILTIN::VDIST(ped1Position.x, ped1Position.y, ped1Position.z, ped2Position.x, ped2Position.y, ped2Position.z);

                // Certifique-se de que ped2 sempre siga atrás de ped1, mantendo uma distância segura
                if (distance > minDistance) {
                    // Se ped2 estiver longe, segue ped1
                    TASK::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(ped2, ped1, 0.0f, -safeDistance, 0.0f, 1.001f, -1, 0.500f, true, false, false, false, false, false);
                    logMessage("Ped2 está seguindo ped1 a uma distância segura.");
                }
                else {
                    // Se ped2 estiver muito perto, desacelere para evitar bloqueio
                    TASK::TASK_STAND_STILL(ped2, 500); // Espera 500ms para manter distância
                    logMessage("Ped2 está muito próximo, ajustando para evitar bloqueio.");
                }
            }
            else {
                // Se o ponto não for navegável, exclui os pedestres e cavalos e reinicia o script
                logMessage("Coordenada não é navegável, excluindo pedestres e cavalos e reiniciando o script.");

                // Excluir os pedestres ped1 e ped2
                if (ENTITY::DOES_ENTITY_EXIST(ped1)) {
                    ENTITY::DELETE_ENTITY(&ped1);
                    logMessage("Ped1 excluído.");
                }
                if (ENTITY::DOES_ENTITY_EXIST(ped2)) {
                    ENTITY::DELETE_ENTITY(&ped2);
                    logMessage("Ped2 excluído.");
                }

                // Excluir os cavalos horse1 e horse2
                if (ENTITY::DOES_ENTITY_EXIST(horse1)) {
                    ENTITY::DELETE_ENTITY(&horse1);
                    logMessage("Horse1 excluído.");
                }
                if (ENTITY::DOES_ENTITY_EXIST(horse2)) {
                    ENTITY::DELETE_ENTITY(&horse2);
                    logMessage("Horse2 excluído.");
                }

                // Reiniciar o script
                logMessage("Reiniciando o script.");
                MoveMountedPedsToCoords(speed, enableInteraction, aggressivePersonality);
            }
        }
    }
}
*/

void func_301()
{
    if (func_300(sLocal_88[0 /*5*/], 0))
    {
        func_160(iLocal_115[2]);
        ENTITY::SET_ENTITY_IS_TARGET_PRIORITY(sLocal_88[0 /*5*/], true, 0.0f);
        ENTITY::CLEAR_ENTITY_LAST_DAMAGE_ENTITY(sLocal_88[0 /*5*/]);
        PED::_CLEAR_PED_DESIRED_LOCO_MOTION_TYPE(sLocal_88[0 /*5*/]);
        if (func_209((1 << 14)))
        {
            TASK::_TASK_INTIMIDATED_2(sLocal_88[0 /*5*/], Global_35, 0, true, true, false, false, true, 0);
        }
        else
        {
            TASK::_TASK_INTIMIDATED_2(sLocal_88[0 /*5*/], Global_35, 0, true, true, true, false, true, 0);
            PED::FORCE_PED_AI_AND_ANIMATION_UPDATE(sLocal_88[0 /*5*/], false, false);
        }
        PED::_SET_PED_TARGET_ACTION_DISABLE_FLAG(sLocal_88[0 /*5*/], 31);
        PED::SET_PED_CONFIG_FLAG(sLocal_88[0 /*5*/], 431 /*PCF_0xA5CB7C09*/, true);
        PED::_SET_PED_INTERACTION_PERSONALITY(sLocal_88[0 /*5*/], joaat("SCRIPTEDINTIMIDATION"));
        PED::SET_PED_CONFIG_FLAG(sLocal_88[0 /*5*/], 178 /*PCF_AllowNonTempExceptionEvents*/, true);
        PED::SET_PED_CONFIG_FLAG(sLocal_88[0 /*5*/], 315 /*PCF_0xD38AEF95*/, false);
        PED::SET_PED_CONFIG_FLAG(sLocal_88[0 /*5*/], 130 /*PCF_DisableTalkTo*/, false);
        PED::SET_PED_CONFIG_FLAG(sLocal_88[0 /*5*/], 301 /*PCF_DisableInteractionLockonOnTargetPed*/, false);
        PED::SET_PED_CONFIG_FLAG(sLocal_88[0 /*5*/], 331 /*PCF_0x9663C8F2*/, false);
        PED::SET_PED_CONFIG_FLAG(sLocal_88[0 /*5*/], 297 /*PCF_ForceInteractionLockonOnTargetPed*/, false);
        PED::SET_PED_CONFIG_FLAG(sLocal_88[0 /*5*/], 452 /*PCF_0x9909D028*/, true);
        TASK::_0x41D1331AFAD5A091(sLocal_88[0 /*5*/], 1, 0);
        TASK::_0x41D1331AFAD5A091(sLocal_88[0 /*5*/], 2, 0);
        PED::_SET_PED_MOTIVATION_STATE_OVERRIDE(sLocal_88[0 /*5*/], 2, false);
        PLAYER::_MODIFY_PLAYER_UI_PROMPT(PLAYER::GET_PLAYER_INDEX(), 8, 0, true);
        PED::SET_PED_DEFENSIVE_AREA_VOLUME(Global_35, iLocal_122[3], false, false, false);
        func_628();
        iLocal_156 = 0;
    }
}










// Variáveis de controle para inputs adicionais
int m_ControlIndex = INPUT_GROUP_KEYBOARD;
bool m_OpenKeyPressed = false;
bool m_BackKeyPressed = false;
bool m_EnterKeyPressed = false;
bool m_UpKeyPressed = false;
bool m_DownKeyPressed = false;
bool m_LeftKeyPressed = false;
bool m_RightKeyPressed = false;
bool m_LMBPressed = false;

// Função para resetar as variáveis do menu
void resetVars()
{
    // Se o submenu não estiver fechado, exibe o índice da opção
    if (submenu != Closed) {
        displayOptionIndex(); // Chama a função para exibir o índice
    }

    // Reseta as flags de pressão de botão
    optionPress = false;
    rightPress = false;
    leftPress = false;
    fastRightPress = false;
    fastLeftPress = false;
    menuHeader = false; // Reseta a flag do cabeçalho

    // Reseta os novos inputs
    m_OpenKeyPressed = false;
    m_BackKeyPressed = false;
    m_EnterKeyPressed = false;
    m_UpKeyPressed = false;
    m_DownKeyPressed = false;
    m_LeftKeyPressed = false;
    m_RightKeyPressed = false;
    m_LMBPressed = false; // Reseta a flag de clique esquerdo do mouse
}

// Função que monitora os botões pressionados para navegação no menu
void ButtonMonitoring()
{
    m_ControlIndex = PAD::IS_USING_KEYBOARD_AND_MOUSE(0) ? 0 : 2; // Definindo o índice de controle

    // Verifica os inputs
    m_OpenKeyPressed = IsKeyJustUp(VK_F7) || (PAD::IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_RB) && PAD::IS_CONTROL_JUST_RELEASED(2, INPUT_CONTEXT_A));
    m_EnterKeyPressed = PAD::IS_CONTROL_JUST_RELEASED(m_ControlIndex, INPUT_GAME_MENU_ACCEPT);
    m_BackKeyPressed = PAD::IS_CONTROL_JUST_RELEASED(m_ControlIndex, INPUT_GAME_MENU_CANCEL);
    m_UpKeyPressed = PAD::IS_CONTROL_JUST_PRESSED(m_ControlIndex, INPUT_GAME_MENU_UP);
    m_DownKeyPressed = PAD::IS_CONTROL_JUST_PRESSED(m_ControlIndex, INPUT_GAME_MENU_DOWN);
    m_LeftKeyPressed = PAD::IS_DISABLED_CONTROL_JUST_PRESSED(m_ControlIndex, INPUT_GAME_MENU_LEFT);
    m_RightKeyPressed = PAD::IS_DISABLED_CONTROL_JUST_PRESSED(m_ControlIndex, INPUT_GAME_MENU_RIGHT);

    if (submenu == Closed) // Se o menu estiver fechado
    {
        if (m_OpenKeyPressed)
        {
            // Abre o menu principal e reseta as variáveis
            submenu = Main_Menu;
            submenuLevel = 0;
            currentOption = 1;
            currentMenuMinOptions = 1;
            currentMenuMaxOptions = maxOptions;
        }
    }
    else // Se o menu estiver aberto
    {
        // Desativa controles enquanto o menu estiver aberto
        if (m_ControlIndex == INPUT_GROUP_GAMEPAD) {
            *getGlobalPtr(1900383 + 316) = 2; // Desabilita o assobio do cavalo nesta frame
            PAD::DISABLE_CONTROL_ACTION(INPUT_GROUP_GAMEPAD, INPUT_WHISTLE, false);
            PAD::DISABLE_CONTROL_ACTION(INPUT_GROUP_GAMEPAD, INPUT_WHISTLE_HORSEBACK, false);
        }

        // Desativa os controles especificados
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_SELECT_RADAR_MODE, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_REVEAL_HUD, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_PLAYER_MENU, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_OPEN_JOURNAL, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_OPEN_SATCHEL_MENU, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_OPEN_SATCHEL_HORSE_MENU, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_OPEN_CRAFTING_MENU, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_OPEN_EMOTE_WHEEL, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_OPEN_WHEEL_MENU, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_EXPAND_RADAR, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_INTERACTION_MENU, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_HUD_SPECIAL, false);

        // Verifica se a tecla Enter foi pressionada
        if (m_EnterKeyPressed) {
            optionPress = true; // Marca que uma opção foi pressionada
        }

        // Verifica se a tecla de voltar foi pressionada
        if (m_BackKeyPressed) {
            if (submenu == Main_Menu) {
                CloseMenu(); // Fecha o menu se estiver no menu principal
            }
            else {
                // Retorna ao último submenu e restaura as opções correspondentes
                submenu = lastSubmenu[submenuLevel - 1];
                currentOption = lastOption[submenuLevel - 1];
                currentMenuMinOptions = lastSubmenuMinOptions[submenuLevel - 1];
                currentMenuMaxOptions = lastSubmenuMaxOptions[submenuLevel - 1];
                submenuLevel--; // Decrementa o nível do submenu
            }
        }

        // Verifica se a tecla para cima foi pressionada
        if (m_UpKeyPressed) {
            if (currentOption == 1) {
                // Muda para a última opção
                currentOption = optionCount;
                currentMenuMaxOptions = optionCount;
                if (optionCount > maxOptions)
                    currentMenuMinOptions = optionCount - maxOptions + 1;
                else
                    currentMenuMinOptions = 1;
            }
            else {
                currentOption--;
                if (currentOption < currentMenuMinOptions) {
                    currentMenuMinOptions = currentOption;
                    currentMenuMaxOptions = currentOption + maxOptions - 1;
                }
            }
        }

        // Verifica se a tecla para baixo foi pressionada
        if (m_DownKeyPressed) {
            if (currentOption == optionCount) {
                // Muda para a primeira opção
                currentOption = 1;
                currentMenuMinOptions = 1;
                currentMenuMaxOptions = maxOptions;
            }
            else {
                currentOption++;
                if (currentOption > currentMenuMaxOptions) {
                    currentMenuMaxOptions = currentOption;
                    currentMenuMinOptions = currentOption - maxOptions + 1;
                }
            }
        }

        // Verifica se a tecla para a esquerda foi pressionada
        if (m_LeftKeyPressed) {
            leftPress = true; // Marca que a tecla esquerda foi pressionada
        }

        // Verifica se a tecla para a direita foi pressionada
        if (m_RightKeyPressed) {
            rightPress = true; // Marca que a tecla direita foi pressionada
        }
    }
}

/*
// Lista de fontes disponíveis
const std::vector<const char*> FontList = { "body", "body1", "catalog1", "catalog2", "catalog3", "catalog4", "catalog5", "chalk",
"Debug_BOLD", "FixedWidthNumbers", "Font5", "gamername", "handwritten", "ledger", "RockstarTAG", "SOCIAL_CLUB_COND_BOLD", "title", "wantedPostersGeneric" };

// Função para desenhar uma sprite (imagem 2D) na tela
void NTDrawSprite(const char* category, const char* sprite, float x, float y, float scalex, float scaley, float rotation, int r, int g, int b, int a)
{
    // Calcula as coordenadas do centro da sprite
    float fX = x + scalex / 2;
    float fY = y + scaley / 2;

    // Verifica se a textura da sprite foi carregada, se não, solicita o carregamento
    if (!TXD::HAS_STREAMED_TEXTURE_DICT_LOADED(sprite))
        TXD::REQUEST_STREAMED_TEXTURE_DICT(sprite, 0);

    // Desenha a sprite na tela com os parâmetros fornecidos
    GRAPHICS::DRAW_SPRITE(category, sprite, fX, fY, scalex, scaley, rotation, r, g, b, a, 1);

    // Libera a textura após ser usada, marcando-a como não mais necessária
    TXD::SET_STREAMED_TEXTURE_DICT_AS_NO_LONGER_NEEDED(category);
}

// Função para desenhar um texto na tela com fonte personalizada
void NTdraw_TextWithFont(const char* text, float x, float y, int r, int g, int b, int a, int fontIndex, bool centered = false, float sx = 0.342f, float sy = 0.342f)
{
    // Define a cor do texto com base nos parâmetros RGBA fornecidos
    UIDEBUG::_BG_SET_TEXT_COLOR(r, g, b, a);

    // Define a escala do texto (tamanho) com base nos valores de sx e sy
    UIDEBUG::_BG_SET_TEXT_SCALE(sx, sy);

    // Define a fonte com base no índice fornecido
    const char* fontName = FontList[fontIndex];

    // Cria o texto formatado com a fonte selecionada
    std::string formatText = "<FONT FACE='$" + std::string(fontName) + "'>~s~" + std::string(text) + "</FONT>";

    // Define se o texto deve ser centralizado
    invoke<Void>(0xBE5261939FBECB8C, centered);

    // Converte o texto para um formato literal string (usado pelo jogo)
    const char* literalString = MISC::VAR_STRING(10, "LITERAL_STRING", formatText.c_str());

    // Desenha o texto nas coordenadas fornecidas (x, y)
    UIDEBUG::_BG_DISPLAY_TEXT(literalString, x, y);
}

// Função para desenhar um retângulo colorido na tela
void NTdrawRect(float x, float y, float width, float height, int r, int g, int b, int a)
{
    // Calcula as coordenadas do centro do retângulo
    float fX = x + width / 2;
    float fY = y + height / 2;

    // Desenha o retângulo na tela com as dimensões e cor especificadas
    GRAPHICS::DRAW_RECT(fX, fY, width, height, r, g, b, a, true, true);
}

// Função para exibir uma notificação personalizada quando o jogador entra no jogo
void ShowWelcomeNotification()
{
    // Dimensões e posição da notificação
    float NTX = 0.052f;       // Posição X (centro da tela)

    // Desenha o fundo da notificação (retângulo)
    NTdrawRect(NTX, 0.050f, 0.260f, 0.212f, 0, 0, 0, 255);

    // Desenha o ícone (sprite)
    // NTDrawSprite("generic_textures", "hud_menu_4a", NTX, 0.131f + 0.027f, 0.260f, 0.002f, 0, 255, 255, 255, 255);

    // Configurações de titulo
    const char* titleNT = "QuickDraw Immersion V2.0!";

    // Desenha o titulo
    NTdraw_TextWithFont(titleNT, NTX + 0.13f, 0.066f, 255, 255, 255, 255, 16, true, 0.6f, 0.6f);

    // Configurações de texto
    const char* textNT1 = "QuickDraw Immersion V2.0!";
    const char* textNT2 = "QuickDraw Immersion V2.0!";
    const char* textNT3 = "QuickDraw Immersion V2.0!";
    const char* textNT4 = "QuickDraw Immersion V2.0!";
    const char* textNT5 = "QuickDraw Immersion V2.0!";

    // Desenha textos
    NTdraw_TextWithFont(textNT1, NTX + 0.13f, 0.096f, 255, 255, 255, 255, 16, true, 0.4f, 0.4f);
    NTdraw_TextWithFont(textNT2, NTX + 0.13f, 0.126f, 255, 255, 255, 255, 16, true, 0.4f, 0.4f);
    NTdraw_TextWithFont(textNT3, NTX + 0.13f, 0.146f, 255, 255, 255, 255, 16, true, 0.4f, 0.4f);
    NTdraw_TextWithFont(textNT4, NTX + 0.13f, 0.166f, 255, 255, 255, 255, 16, true, 0.4f, 0.4f);
    NTdraw_TextWithFont(textNT5, NTX + 0.13f, 0.186f, 255, 255, 255, 255, 16, true, 0.4f, 0.4f);
}
*/