#pragma once

Blip createBlip(Vector3 pos, Hash blipType, Hash blipSprite = 0);
Blip createBlip(Entity entity, Hash blipType, Hash blipSprite = 0);
Blip createBlip(Vector3 source, float radius, Hash blipType, Hash blipSprite = 0);
void setBlipLabel(Blip blip, const char* label);
void deleteBlipSafe(Blip* blip);
Object createProp(const char* model, Vector3 position, float heading, bool isStatic, bool isVisible);
void ManagePedBlip(Ped ped, Player player);
void ManageAllPedBlips(Player player);
