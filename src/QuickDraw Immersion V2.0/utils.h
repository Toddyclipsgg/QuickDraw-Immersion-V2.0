#pragma once

#include <vector>
#include <unordered_map>

Hash joaat(const char* string);
float CalculateDistance(const Vector3& pos1, const Vector3& pos2);
float DistanceBetweenEntitiesHor(Entity entity1, Entity entity2);
bool DeleteEntitiesOnCondition(std::vector<Entity>& entities, float maxDistance, bool deleteOnPlayerDeath);
bool ShouldDeleteEntity(Entity entity, Player playerPed, std::unordered_map<Entity, int>& pedStopTimes);
bool CheckEntityObstacles(Entity ped);
bool HandleEntityInteractions(Entity ped, Player player);
bool HandleCombatStatus(Entity ped, Player player);
bool DeleteVehiclesAndHorses(std::vector<Entity>& entities, float maxDistance, bool deleteOnPlayerDeath);
bool ShouldDeleteVehicleOrHorse(Entity entity);
void DeleteVehiclesAndHorsesDead(std::vector<Entity>& entities, float maxDistance);
bool DeleteEntities(std::vector<Entity>& entities, int batchSize);
