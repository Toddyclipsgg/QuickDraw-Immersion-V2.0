#include "..\header.h"

std::vector<std::string> availableHorseHashList = horseHashList;  // Lista de cavalos disponíveis para gerar
std::vector<std::string> usedHorseHashList;  // Lista de cavalos já utilizados
static std::vector<std::pair<std::string, Vector3>> availableCoordList = coordenadas;
static std::vector<std::pair<std::string, Vector3>> usedCoordList;

// Função para gerar um cavalo aleatório da lista e remover o modelo após o uso
std::string GetRandomHorseHash() {
	// Semente para o gerador de números aleatórios
	std::srand(static_cast<unsigned int>(std::time(0)));

	// Verificar se ainda há cavalos disponíveis na lista
	if (availableHorseHashList.empty()) {
		// Reiniciar a lista se todos os cavalos já foram gerados
		availableHorseHashList = horseHashList;
		usedHorseHashList.clear();  // Limpa os cavalos usados
	}

	// Obter um índice aleatório
	int randomIndex = std::rand() % availableHorseHashList.size();

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
	// Semente para o gerador de números aleatórios
	std::srand(static_cast<unsigned int>(std::time(0)));

	// Verificar se ainda há coordenadas disponíveis
	if (availableCoordList.empty()) {
		// Reiniciar a lista se todas as coordenadas já foram usadas
		availableCoordList = coordenadas;
		usedCoordList.clear(); // Limpa as coordenadas usadas
	}

	// ID do jogador e coordenadas atuais
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);

	// Variáveis para armazenar a coordenada mais distante
	std::pair<std::string, Vector3> farthestCoord;
	float maxDistance = -1.0f;

	// Encontrar a coordenada mais distante do jogador
	for (const auto& coord : availableCoordList) {
		float distance = MISC::GET_DISTANCE_BETWEEN_COORDS(
			playerPos.x, playerPos.y, playerPos.z,
			coord.second.x, coord.second.y, coord.second.z, true);

		// Se a distância for maior que a máxima encontrada, atualizar
		if (distance > maxDistance) {
			maxDistance = distance;
			farthestCoord = coord;
		}
	}

	// Mover a coordenada selecionada para a lista de usadas
	usedCoordList.push_back(farthestCoord);

	// Encontrar e remover a coordenada mais distante da lista disponível
	auto it = std::find_if(availableCoordList.begin(), availableCoordList.end(),
		[&farthestCoord](const std::pair<std::string, Vector3>& coord) {
			return coord.first == farthestCoord.first &&
				coord.second.x == farthestCoord.second.x &&
				coord.second.y == farthestCoord.second.y &&
				coord.second.z == farthestCoord.second.z;
		});
	if (it != availableCoordList.end()) {
		availableCoordList.erase(it);
	}

	// Retornar a coordenada mais distante encontrada
	return farthestCoord;
}

bool isPlayerWithinDistance(double maxDistance) {
	// Obter a entidade do jogador (Ped)
	Ped player_ped = PLAYER::PLAYER_PED_ID();

	// Obter as coordenadas atuais do jogador
	Vector3 player_coords = ENTITY::GET_ENTITY_COORDS(player_ped, true, true);

	// Iterar sobre cada local na lista e verificar a distância
	for (const auto& location : locations) {
		double distance = BUILTIN::VDIST(
			static_cast<float>(player_coords.x), static_cast<float>(player_coords.y), static_cast<float>(player_coords.z),
			static_cast<float>(location.coords.x), static_cast<float>(location.coords.y), static_cast<float>(location.coords.z)
		);

		// Verificar se a distância é menor ou igual à distância máxima
		if (distance <= maxDistance) {
			logMessage("Player is near " + location.name + ". Distance: " + std::to_string(distance));
			return true;
		}
	}

	// Se não estiver perto de nenhuma localização, retornar false
	return false;
}

// Função para verificar se a entidade está dentro do raio proibido
bool IsSpawnRestricted(const Vector3& spawnCoords) {
	const float restrictedRadius = 500.0f; // Raio de 500 metros
	for (const auto& location : locations) {
		float distance = BUILTIN::VDIST(
			spawnCoords.x, spawnCoords.y, spawnCoords.z,
			location.coords.x, location.coords.y, location.coords.z
		);
		if (distance <= restrictedRadius) {
			logMessage("Spawn restriction active near location: " + std::string(location.name));
			return true; // Spawn está proibido
		}
	}
	return false; // Fora da área de restrição
}
