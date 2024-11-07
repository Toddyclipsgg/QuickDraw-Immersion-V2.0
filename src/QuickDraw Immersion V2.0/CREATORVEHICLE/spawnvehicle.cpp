#include "..\header.h"

namespace VehicleSpawner {

// Função principal que cria um veículo com base no nome ou hash do modelo
Vehicle spawnVehicle(const char* vehicleModel, Hash modelHash, Vector3 position, float heading)
{
    // Se o hash não for fornecido, obtém o hash a partir do nome do modelo
    if (modelHash == 0 && vehicleModel != nullptr)
    {
        modelHash = MISC::GET_HASH_KEY(vehicleModel);
    }

    // Verifica se o modelo está carregado, caso contrário, solicita o carregamento
    if (!STREAMING::HAS_MODEL_LOADED(modelHash))
    {
        STREAMING::REQUEST_MODEL(modelHash, false);
        while (!STREAMING::HAS_MODEL_LOADED(modelHash))
        {
            WAIT(0);
        }
    }

    // Cria o veículo na posição e direção especificadas
    Vehicle vehicle = VEHICLE::CREATE_VEHICLE(
        modelHash, position.x, position.y, position.z, heading, true, true, false, false);

    // Garante que o veículo seja corretamente colocado no solo
    VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicle, 0);

    return vehicle;
}

// Sobrecargas para criar veículo usando nome ou hash
Vehicle spawnVehicle(const char* vehicleModel, Vector3 position, float heading)
{
    return spawnVehicle(vehicleModel, 0, position, heading);
}

Vehicle spawnVehicle(Hash modelHash, Vector3 position, float heading)
{
    return spawnVehicle(nullptr, modelHash, position, heading);
}

// Função para adicionar uma carga ao veículo
void attachCargoToVehicle(Vehicle vehicle, VehicleCargoHash cargoType)
{
    PROPSET::_ADD_PROP_SET_FOR_VEHICLE(vehicle, cargoType);
}

} // namespace VehicleSpawner
