#pragma once

#pragma once

namespace QuickDrawImmersion {

Hash getRandomSaddleComboHash();
Ped pedMount(Hash pedHash, Ped horse, bool seatIndex, Vector3 sourcePosition, int radius);
Ped createPed(Hash pedModelHash, Vector3 pos, float heading = 0);
void giveComboSaddleToHorse(Ped horse);
void giveSaddleToHorse(Ped horse, Hash saddleHash);
Ped createPedOnHorse(Hash pedModelHash, Ped horse, int seatIndex = 1);

}
