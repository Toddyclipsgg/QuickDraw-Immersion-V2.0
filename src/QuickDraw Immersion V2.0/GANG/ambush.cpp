#include "..\header.h"
#include "ambush.h"

// Inicialização da variável global
// Remover variável global g_hashCache

Vector3 AmbushManager::GetPlayerPosition() {
    Ped playerPed = PLAYER::PLAYER_PED_ID();
    return ENTITY::GET_ENTITY_COORDS(playerPed, true, false);
}

Vector3 AmbushManager::GetRandomAmbushLocation() {
    logMessage("Calculando localização aleatória para emboscada.");
    Vector3 playerPos = GetPlayerPosition();
    
    // Aumentar variação da distância para emboscadas mais imprevisíveis
    float minDistance = 10.0f;
    float maxDistance = 20.0f;
    float distance = minDistance + static_cast<float>(rand()) / RAND_MAX * (maxDistance - minDistance);
    
    // Usar mais pontos cardeais para melhor distribuição
    float angle = static_cast<float>(rand()) / RAND_MAX * 6.28318f;
    
    Vector3 location = {
        playerPos.x + cos(angle) * distance,
        playerPos.y + sin(angle) * distance,
        playerPos.z
    };

    // Ajustar altura do terreno
    float groundZ;
    MISC::GET_GROUND_Z_FOR_3D_COORD(location.x, location.y, location.z + 100.0f, &groundZ, false);
    location.z = groundZ;

    return location;
}

Hash AmbushManager::GetCachedHash(const char* key, bool isWeapon) {
    std::string sKey(key);
    auto& cache = isWeapon ? m_hashCache.weaponHashes : m_hashCache.modelHashes;

    auto it = cache.find(sKey);
    if (it != cache.end()) {
        return it->second;
    } else {
        Hash hash = MISC::GET_HASH_KEY(key);
        cache[sKey] = hash;
        return hash;
    }
}

AmbushManager::AmbushManager() : m_ambushActive(false) {
    logMessage("Inicializando AmbushManager.");
    m_config = {
        80.0f,                    // triggerDistance
        3,                        // minEnemies
        6,                        // maxEnemies
        15.0f,                    // spawnRadius
        {                         // enemyModels
            "G_M_M_UNIDUSTER_01",
            "G_M_M_UNIDUSTER_02",
            "G_M_M_UNIDUSTER_03"
        },
        {                         // weaponTypes
            "WEAPON_REVOLVER",
            "WEAPON_REPEATER",
            "WEAPON_SHOTGUN"
        }
    };
    // Inicializar HashCache
    m_hashCache = {};
}

AmbushManager::~AmbushManager() {
    Cleanup();
}

void AmbushManager::Update() {
    if (!m_ambushActive && ShouldTriggerAmbush()) {
        TriggerAmbush();
    } else if (m_ambushActive) {
        ManageActiveAmbush();
    }
}

bool AmbushManager::ShouldTriggerAmbush() {
    logMessage("Verificando se deve iniciar emboscada.");
    static uint32_t lastCheck = 0;
    uint32_t currentTime = GetTickCount();
    
    if (currentTime - lastCheck < 1000) return false;
    lastCheck = currentTime;

    Vector3 playerPos = GetPlayerPosition();
    Vector3 ambushLoc = GetRandomAmbushLocation();
    
    return CalculateDistance1(playerPos, ambushLoc) <= m_config.triggerDistance;
}

void AmbushManager::PlayAmbushSound() {
    logMessage("Tocando som de emboscada.");
    AUDIO::_PLAY_SOUND_FROM_POSITION("AMBUSH_START", m_ambushCenter.x, m_ambushCenter.y, m_ambushCenter.z, "AMBUSH_SOUNDSET", false, 0, true, 0);
}

void AmbushManager::TriggerEnemyDialogues() {
    logMessage("Iniciando diálogos dos inimigos.");
    for (Entity e : m_activeEntities) {
        Ped npc = static_cast<Ped>(e);
        if (ENTITY::DOES_ENTITY_EXIST(npc) && !ENTITY::IS_ENTITY_DEAD(npc)) {
            TASK::TASK_PLAY_ANIM(npc, "random@ambush", "ambush_shout", 8.0f, -8.0f, -1, 0, 0, false, 0, false, false, false);
        }
    }
}

void AmbushManager::CreateVisualEffects() {
    logMessage("Criando efeitos visuais para a emboscada.");
    Vector3 effectPos = m_ambushCenter;
    GRAPHICS::START_PARTICLE_FX_NON_LOOPED_AT_COORD("EXPLOSION_TRAIL", effectPos.x, effectPos.y, effectPos.z, 0.0f, 0.0f, 0.0f, 1.0f, false, false, false);
}

void AmbushManager::SpawnReinforcements() {
    logMessage("Spawning enemy reinforcements.");
    int reinforcementCount = rand() % 3 + 2; // Spawnar 2-4 inimigos adicionais
    for (int i = 0; i < reinforcementCount; i++) {
        Vector3 spawnPos = GetRandomAmbushLocation();
        const char* enemyModel = m_config.enemyModels[rand() % m_config.enemyModels.size()];
        const char* weapon = m_config.weaponTypes[rand() % m_config.weaponTypes.size()];
        Ped npc = SpawnEnemy(enemyModel, spawnPos, weapon);
        m_activeEntities.push_back(npc);
        
        logMessage("Reforço inimigo gerado.");
    }

    NotifyPlayer("Novos inimigos estão chegando!");
}

void AmbushManager::NotifyPlayer(const char* message) {
    logMessage("Enviando notificação ao jogador.");
    // Converte o texto para um formato literal string (usado pelo jogo)
    const char* literalString = MISC::VAR_STRING(10, "LITERAL_STRING", message);

    // Exibe a legenda no log da interface do usuário (UILOG) do jogo
    UILOG::_UILOG_SET_CACHED_OBJECTIVE(literalString); // Adiciona a legenda
    UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();              // Atualiza o log
    UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();              // Exibe a legenda
}

void AmbushManager::TriggerAmbush() {
    logMessage("Iniciando sequência de emboscada.");
    m_ambushActive = true;
    
    m_ambushCenter = GetRandomAmbushLocation();
    NotifyPlayer("Uma emboscada está acontecendo!");

    Vector3 ambushCenter = m_ambushCenter;
    int enemyCount = m_config.minEnemies + rand() % (m_config.maxEnemies - m_config.minEnemies + 1);

    // Spawn inimigos em formação tática
    for (int i = 0; i < enemyCount; i++) {
        float angle = (static_cast<float>(i) / enemyCount) * 6.28318f;
        Vector3 spawnPos = {
            ambushCenter.x + cos(angle) * m_config.spawnRadius,
            ambushCenter.y + sin(angle) * m_config.spawnRadius,
            ambushCenter.z
        };

        const char* enemyModel = m_config.enemyModels[rand() % m_config.enemyModels.size()];
        const char* weapon = m_config.weaponTypes[rand() % m_config.weaponTypes.size()];

        Ped npc = SpawnEnemy(enemyModel, spawnPos, weapon);
        m_activeEntities.push_back(npc);
    }

    // Adicionar bloqueio de rotas
    SpawnRoadBlocks(ambushCenter);

    // Melhorias de imersão
    PlayAmbushSound();
    TriggerEnemyDialogues();
    CreateVisualEffects();

    // Agendar reforços após 10 segundos
    WAIT(10000); // Esperar 10 segundos
    SpawnReinforcements();
}

Ped AmbushManager::SpawnEnemy(const char* model, Vector3 pos, const char* weapon) {
    logMessage("Gerando inimigo.");
    Hash modelHash = GetCachedHash(model);
    Hash weaponHash = GetCachedHash(weapon, true);
    
    STREAMING::REQUEST_MODEL(modelHash, false);
    while (!STREAMING::HAS_MODEL_LOADED(modelHash)) WAIT(0);

    Ped npc = PED::CREATE_PED(modelHash, pos.x, pos.y, pos.z, 0.0f, false, false, false, false);
    ManagePedBlip(npc, PLAYER::PLAYER_ID());

    // Melhorar IA e comportamento do NPC
    PED::SET_PED_COMBAT_ABILITY(npc, 100);
    PED::SET_PED_COMBAT_ATTRIBUTES(npc, 46, true);
    PED::SET_PED_ACCURACY(npc, 80);
    PED::SET_PED_COMBAT_MOVEMENT(npc, 2);
    
    // Adicionar variação nas armas e munição
    int ammo = 50 + rand() % 100;
    WEAPON::GIVE_WEAPON_TO_PED(npc, weaponHash, ammo, true, true, 0, true, 0.5f, 1.0f, 
        MISC::GET_HASH_KEY("AMMO_REASON_DEFAULT"), false, 0.5f, 0);
    
    // Melhorar pathfinding e comportamento tático
    TASK::SET_PED_PATH_CAN_USE_CLIMBOVERS(npc, true);
    TASK::SET_PED_PATH_CAN_USE_LADDERS(npc, true);
    TASK::SET_PED_PATH_AVOID_FIRE(npc, true);
    TASK::SET_PED_PATH_PREFER_TO_AVOID_WATER(npc, true, 1.0f);
    
    // Configurar cobertura e combate
    TASK::TASK_COMBAT_PED(npc, PLAYER::PLAYER_PED_ID(), 0, 16);
    PED::SET_PED_COMBAT_RANGE(npc, 2);
    
    STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(modelHash);
    return npc;
}

void AmbushManager::ManageActiveAmbush() {
    logMessage("Gerenciando emboscada ativa.");
    m_activeEntities.erase(
        std::remove_if(m_activeEntities.begin(), m_activeEntities.end(),
            [](Entity e) {
                if (!ENTITY::DOES_ENTITY_EXIST(e) || ENTITY::IS_ENTITY_DEAD(e)) {
                    ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&e);
                    return true;
                }
                return false;
            }
        ),
        m_activeEntities.end()
    );

    if (m_activeEntities.empty()) {
        m_ambushActive = false;
        logMessage("Emboscada finalizada.");
    }
}

void AmbushManager::SpawnRoadBlocks(Vector3 center) {
    logMessage("Gerando bloqueios de estrada.");
    
    const char* blockObjects[] = {
        "P_BARRICADE01X",
        "P_BARRICADE02X",
        "P_BARRICADE03X"
    };

    for (int i = 0; i < 4; i++) {
        float angle = (static_cast<float>(i) / 4) * 6.28318f;
        Vector3 blockPos = {
            center.x + cos(angle) * 15.0f,
            center.y + sin(angle) * 15.0f,
            center.z
        };

        // Ajustar altura do terreno
        float groundZ;
        MISC::GET_GROUND_Z_FOR_3D_COORD(blockPos.x, blockPos.y, blockPos.z + 100.0f, &groundZ, false);
        blockPos.z = groundZ;

        // Spawn bloqueio aleatório
        const char* blockModel = blockObjects[rand() % 3];
        Hash modelHash = GetCachedHash(blockModel);
        
        STREAMING::REQUEST_MODEL(modelHash, false);
        while (!STREAMING::HAS_MODEL_LOADED(modelHash)) WAIT(0);

        Object block = OBJECT::CREATE_OBJECT(modelHash, blockPos.x, blockPos.y, blockPos.z, 
            true, true, false, false, true);
        
        if (ENTITY::DOES_ENTITY_EXIST(block)) {
            ENTITY::SET_ENTITY_HEADING(block, angle * 57.2958f);
            m_activeEntities.push_back(block);
        }
        
        STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(modelHash);
    }
}

void AmbushManager::Cleanup() {
    for (Entity e : m_activeEntities) {
        if (ENTITY::DOES_ENTITY_EXIST(e)) {
            ENTITY::DELETE_ENTITY(&e);
        }
    }
    m_activeEntities.clear();
    m_ambushActive = false;

    // Limpar caches de hashes
    m_hashCache.modelHashes.clear();
    m_hashCache.weaponHashes.clear();
}