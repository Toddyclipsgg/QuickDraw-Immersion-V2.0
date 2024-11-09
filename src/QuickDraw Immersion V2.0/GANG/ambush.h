#pragma once
#include <vector>
#include <unordered_map>
#include <string>

// Estrutura para configurar emboscadas
struct AmbushConfig {
    float triggerDistance;
    int minEnemies;
    int maxEnemies;
    float spawnRadius;
    std::vector<const char*> enemyModels;
    std::vector<const char*> weaponTypes;
};

// Cache de hash keys para melhor performance
struct HashCache {
    std::unordered_map<std::string, Hash> modelHashes;
    std::unordered_map<std::string, Hash> weaponHashes;
};

class AmbushManager {
private:
    AmbushConfig m_config;
    std::vector<Entity> m_activeEntities;
    bool m_ambushActive;

    // Mover HashCache para dentro da classe
    HashCache m_hashCache;

    Vector3 GetPlayerPosition();
    Vector3 GetRandomAmbushLocation();
    Hash GetCachedHash(const char* key, bool isWeapon = false);

    Vector3 m_ambushCenter; // Adicionar para armazenar o centro da emboscada

    // Novas funções para melhorar a imersão
    void PlayAmbushSound();
    void TriggerEnemyDialogues();
    void CreateVisualEffects();
    void SpawnReinforcements();
    void NotifyPlayer(const char* message);

public:
    AmbushManager();
    ~AmbushManager(); // Adicionar destrutor

    void Update();
    bool ShouldTriggerAmbush();
    void TriggerAmbush();
    Ped SpawnEnemy(const char* model, Vector3 pos, const char* weapon);
    void ManageActiveAmbush();
    void SpawnRoadBlocks(Vector3 center);
    void Cleanup();
};