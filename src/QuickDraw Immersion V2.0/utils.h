#pragma once

#include <vector>

bool DeleteEntitiesOnCondition(std::vector<Entity>& entities, float maxDistance, bool deleteOnPlayerDeath);
bool CheckEntityObstacles(Entity ped);
bool HandleEntityInteractions(Entity ped, Player player);
bool HandleCombatStatus(Entity ped, Player player);
bool DeleteVehiclesAndHorses(std::vector<Entity>& entities, float maxDistance, bool deleteOnPlayerDeath);
float CalcularDistancia(Vector3 pedCoords, Vector3 destinoCoords);
float distanceBetweenEntitiesHor(Entity entity1, Entity entity2);