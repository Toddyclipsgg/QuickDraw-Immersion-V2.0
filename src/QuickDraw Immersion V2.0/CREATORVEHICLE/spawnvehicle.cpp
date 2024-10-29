#include "..\header.h"

// Função para criar um veículo a partir de um nome de modelo
Vehicle spawnVehicleByName(char* vehicleModel, Vector3 position, float heading)
{
    Hash modelHash = MISC::GET_HASH_KEY(vehicleModel);  // Obter o hash do nome do modelo
    return spawnVehicle(modelHash, position, heading); // Chama a função principal para criar o veículo
}

// Função para criar um veículo usando um hash específico de veículo
Vehicle spawnVehicleByHash(VehicleHash vehicleHash, Vector3 position, float heading)
{
    return spawnVehicle((Hash)vehicleHash, position, heading); // Redireciona para a função principal
}

// Função principal que cria um veículo com base no hash do modelo
Vehicle spawnVehicle(Hash modelHash, Vector3 spawnPosition, float heading)
{
    Vehicle vehicleEntity;

    // Verifica se o modelo está carregado, caso contrário, solicita o carregamento
    if (!STREAMING::HAS_MODEL_LOADED(modelHash))
    {
        STREAMING::REQUEST_MODEL(modelHash, false);
    }

    // Aguarda o carregamento completo do modelo
    while (!STREAMING::HAS_MODEL_LOADED(modelHash))
    {
        WAIT(0);
    }

    // Cria o veículo na posição e direção especificada
    vehicleEntity = VEHICLE::CREATE_VEHICLE(modelHash, spawnPosition.x, spawnPosition.y, spawnPosition.z, heading, true, true, false, false);

    // Garante que o veículo seja corretamente colocado no solo
    VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicleEntity, 0);

    // Define o veiculo visivel
    // PED::_SET_RANDOM_OUTFIT_VARIATION(vehicleEntity, true);

    return vehicleEntity; // Retorna a referência do veículo
}

// Função para adicionar uma carga ao veículo
void attachCargoToVehicle(Vehicle vehicle, VehicleCargoHash cargoType)
{
    PROPSET::_ADD_PROP_SET_FOR_VEHICLE(vehicle, cargoType); // Utiliza o hash para definir a carga do veículo
}
