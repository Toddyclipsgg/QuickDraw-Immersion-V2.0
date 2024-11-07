#include "..\header.h"
#include <random>
#include <chrono>

// Random number generator
std::mt19937 randomEngine(std::chrono::steady_clock::now().time_since_epoch().count());

// Lista de cavalos disponíveis para gerar
std::vector<std::string> availableHorseHashList = horseHashList;
std::vector<std::string> usedHorseHashList;  // Lista de cavalos já utilizados

static std::vector<std::pair<std::string, Vector3>> availableCoordList = coordenadas;
static std::vector<std::pair<std::string, Vector3>> usedCoordList;

// Função para gerar um cavalo aleatório da lista e remover o modelo após o uso
std::string GetRandomHorseHash() {
	// Verificar se ainda há cavalos disponíveis na lista
	if (availableHorseHashList.empty()) {
		// Reiniciar a lista se todos os cavalos já foram gerados
		availableHorseHashList = horseHashList;
		usedHorseHashList.clear();  // Limpa os cavalos usados
	}

	// Gerar um índice aleatório
	std::uniform_int_distribution<> dist(0, availableHorseHashList.size() - 1);
	int randomIndex = dist(randomEngine);

	// Obter o hash correspondente ao índice aleatório
	std::string selectedHorseHash = availableHorseHashList[randomIndex];

	// Remover o modelo da memória para evitar duplicação de cor/raça
	Hash horseModel = MISC::GET_HASH_KEY(selectedHorseHash.c_str());
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(horseModel);

	// Mover o cavalo selecionado para a lista de usados
	usedHorseHashList.push_back(selectedHorseHash);

	// Remover o cavalo selecionado da lista para não gerar o mesmo
	availableHorseHashList.erase(availableHorseHashList.begin() + randomIndex);

	// Retornar o hash do cavalo
	return selectedHorseHash;
}

// Função para gerar coordenadas opostas à posição atual do jogador
std::pair<std::string, Vector3> GetRandomCoord() {
	// Verificar se ainda há coordenadas disponíveis
	if (availableCoordList.empty()) {
		// Reiniciar a lista se todas as coordenadas já foram usadas
		availableCoordList = coordenadas;
		usedCoordList.clear();  // Limpa as coordenadas usadas
	}

	// ID do jogador e coordenadas atuais
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);

	// Variáveis para armazenar a coordenada mais distante
	int farthestIndex = -1;
	float maxDistance = -1.0f;

	// Encontrar o índice da coordenada mais distante do jogador
	for (size_t i = 0; i < availableCoordList.size(); ++i) {
		const auto& coord = availableCoordList[i];
		float distance = MISC::GET_DISTANCE_BETWEEN_COORDS(
			playerPos.x, playerPos.y, playerPos.z,
			coord.second.x, coord.second.y, coord.second.z, true);

		// Se a distância for maior que a máxima encontrada, atualizar
		if (distance > maxDistance) {
			maxDistance = distance;
			farthestIndex = static_cast<int>(i);
		}
	}

	// Verificar se uma coordenada foi encontrada
	if (farthestIndex != -1) {
		// Obter a coordenada mais distante
		auto farthestCoord = availableCoordList[farthestIndex];

		// Mover a coordenada selecionada para a lista de usadas
		usedCoordList.push_back(farthestCoord);

		// Remover a coordenada selecionada da lista disponível
		availableCoordList.erase(availableCoordList.begin() + farthestIndex);

		// Retornar a coordenada mais distante encontrada
		return farthestCoord;
	} else {
		// Caso não haja coordenadas disponíveis
		throw std::runtime_error("No available coordinates found.");
	}
}

bool isPlayerWithinDistance(double maxDistance) {
	// Obter a entidade do jogador (Ped)
	Ped player_ped = PLAYER::PLAYER_PED_ID();

	// Obter as coordenadas atuais do jogador
	Vector3 player_coords = ENTITY::GET_ENTITY_COORDS(player_ped, true, true);

	// Iterar sobre cada local na lista e verificar a distância
	for (const auto& location : locations) {
		float distance = BUILTIN::VDIST(
			player_coords.x, player_coords.y, player_coords.z,
			location.coords.x, location.coords.y, location.coords.z
		);

		// Verificar se a distância é menor ou igual à distância máxima
		if (distance <= maxDistance) {
			logMessage("Player is near " + location.name +
					   ". Distance: " + std::to_string(distance));
			return true;
		}
	}

	// Se não estiver perto de nenhuma localização, retornar false
	return false;
}

// Função para verificar se a entidade está dentro do raio proibido
bool IsSpawnRestricted(const Vector3& spawnCoords) {
	const float restrictedRadius = 500.0f;  // Raio de 500 metros
	for (const auto& location : locations) {
		float distance = BUILTIN::VDIST(
			spawnCoords.x, spawnCoords.y, spawnCoords.z,
			location.coords.x, location.coords.y, location.coords.z
		);
		if (distance <= restrictedRadius) {
			logMessage("Spawn restriction active near location: " +
					   std::string(location.name));
			return true;  // Spawn está proibido
		}
	}
	return false;  // Fora da área de restrição
}
