#include "..\header.h"

// Fun��o para criar um ve�culo a partir de um nome de modelo
Vehicle spawnVehicleByName(char* vehicleModel, Vector3 position, float heading)
{
    Hash modelHash = MISC::GET_HASH_KEY(vehicleModel);  // Obter o hash do nome do modelo
    return spawnVehicle(modelHash, position, heading); // Chama a fun��o principal para criar o ve�culo
}

// Fun��o para criar um ve�culo usando um hash espec�fico de ve�culo
Vehicle spawnVehicleByHash(VehicleHash vehicleHash, Vector3 position, float heading)
{
    return spawnVehicle((Hash)vehicleHash, position, heading); // Redireciona para a fun��o principal
}

// Fun��o principal que cria um ve�culo com base no hash do modelo
Vehicle spawnVehicle(Hash modelHash, Vector3 spawnPosition, float heading)
{
    Vehicle vehicleEntity;

    // Verifica se o modelo est� carregado, caso contr�rio, solicita o carregamento
    if (!STREAMING::HAS_MODEL_LOADED(modelHash))
    {
        STREAMING::REQUEST_MODEL(modelHash, false);
    }

    // Aguarda o carregamento completo do modelo
    while (!STREAMING::HAS_MODEL_LOADED(modelHash))
    {
        WAIT(0);
    }

    // Cria o ve�culo na posi��o e dire��o especificada
    vehicleEntity = VEHICLE::CREATE_VEHICLE(modelHash, spawnPosition.x, spawnPosition.y, spawnPosition.z, heading, true, true, false, false);

    // Garante que o ve�culo seja corretamente colocado no solo
    VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicleEntity, 0);

    // Define o veiculo visivel
    // PED::_SET_RANDOM_OUTFIT_VARIATION(vehicleEntity, true);

    return vehicleEntity; // Retorna a refer�ncia do ve�culo
}

// Fun��o para adicionar uma carga ao ve�culo
void attachCargoToVehicle(Vehicle vehicle, VehicleCargoHash cargoType)
{
    PROPSET::_ADD_PROP_SET_FOR_VEHICLE(vehicle, cargoType); // Utiliza o hash para definir a carga do ve�culo
}
