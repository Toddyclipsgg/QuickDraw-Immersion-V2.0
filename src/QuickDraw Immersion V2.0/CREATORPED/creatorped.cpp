#include "..\header.h"

// Função para retornar um Hash aleatório da lista
Hash getComboSaddleRandomHash() {
    // Lista de hashes
    std::vector<Hash> hashList = {
        0xB3C18CAB, // items for saddlecombo_hr1_003
        0xE0FBE71F, // items for saddlecombo_hr1_010
        0xD0144550, // items for saddlecombo_hr1_010
        0xDE748DAC, // items for saddlecombo_hr1_007
        0x404CC574, // items for saddlecombo_hr1_004
        0x4E5E6197, // items for saddlecombo_hr1_005
        0xD038E54E, // items for saddlecombo_hr1_010
        0x9E8601E9, // items for saddlecombo_hr1_003
        0xBB6D3BB7  // items for saddlecombo_hr1_010
    };

    // Inicializar a semente aleatória
    srand(static_cast<unsigned int>(time(0)));

    // Selecionar um hash aleatoriamente
    int randomIndex = rand() % hashList.size();

    return hashList[randomIndex];
}

// Função para montar o ped no cavalo
Ped pedMount(Hash pedHash, Ped horse, bool seatIndex, Vector3 sourcePosition, int radius) {
    // Criar o ped e montá-lo no cavalo
    Ped ped = createPedOnHorse(pedHash, horse, seatIndex ? -1 : 0);
    DECORATOR::DECOR_SET_INT(ped, "GangMount", 0);
    return ped;
}

// Função para criar Peds usando Hash
Ped createPed(Hash pedModelHash, Vector3 pos, float heading)
{

    // Solicitar o modelo Ped (diretamente do enum hash)
    STREAMING::REQUEST_MODEL(pedModelHash, false);

    // Verifique se o modelo está carregado
    while (!STREAMING::HAS_MODEL_LOADED(pedModelHash))
    {
        WAIT(100); // Espera um pouco até o modelo ser carregado
    }

    // Criar Ped
    Ped ped = PED::CREATE_PED(pedModelHash, pos.x, pos.y, pos.z, heading, false, false, false, false);
    PED::_SET_RANDOM_OUTFIT_VARIATION(ped, true);

    return ped;
}

// Função para aplicar sela ao cavalo
void giveComboSaddleToHorse(Ped horse)
{
    Hash comboSaddleRandomHash = getComboSaddleRandomHash();

    if (ENTITY::IS_ENTITY_DEAD(horse) || PED::IS_PED_INJURED(horse))
    {
    }
    PED::_EQUIP_META_PED_OUTFIT(horse, comboSaddleRandomHash);
    if (horse)
    {
        PED::_UPDATE_PED_VARIATION(horse, false, true, true, true, false);
    }
}

// Função para aplicar sela ao cavalo
void giveSaddleToHorse(Ped horse, Hash saddleHash)
{
    if (invoke<Void>(0xD3A7B003ED343FD9, horse, saddleHash, TRUE, FALSE, FALSE))
    {
        logMessage("Erro: Sela não foi aplicada ao cavalo.");
    }
}

// Função para criar ped montado em cavalo
Ped createPedOnHorse(Hash pedModelHash, Ped horse, int seatIndex)
{

    STREAMING::REQUEST_MODEL(pedModelHash, false);
    while (!STREAMING::HAS_MODEL_LOADED(pedModelHash))
    {
        WAIT(100); // Espera até o modelo ser carregado
    }

    Ped ped = PED::CREATE_PED_ON_MOUNT(horse, pedModelHash, seatIndex, 1, 1, 0, 0);
    PED::_SET_RANDOM_OUTFIT_VARIATION(ped, true); // Aplica uma variação aleatória de roupa

    return ped;
}