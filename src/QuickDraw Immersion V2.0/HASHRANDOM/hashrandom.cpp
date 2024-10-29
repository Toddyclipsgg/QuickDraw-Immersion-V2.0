#include "..\header.h"

std::vector<std::string> availableHorseHashList = horseHashList;  // Lista de cavalos dispon�veis para gerar
std::vector<std::string> usedHorseHashList;  // Lista de cavalos j� utilizados
static std::vector<std::pair<std::string, Vector3>> availableCoordList = coordenadas;
static std::vector<std::pair<std::string, Vector3>> usedCoordList;

// Fun��o para gerar um cavalo aleat�rio da lista e remover o modelo ap�s o uso
std::string GetRandomHorseHash() {
	// Semente para o gerador de n�meros aleat�rios
	std::srand(static_cast<unsigned int>(std::time(0)));

	// Verificar se ainda h� cavalos dispon�veis na lista
	if (availableHorseHashList.empty()) {
		// Reiniciar a lista se todos os cavalos j� foram gerados
		availableHorseHashList = horseHashList;
		usedHorseHashList.clear();  // Limpa os cavalos usados
	}

	// Obter um �ndice aleat�rio
	int randomIndex = std::rand() % availableHorseHashList.size();

	// Obter o hash correspondente ao �ndice aleat�rio
	std::string selectedHorseHash = availableHorseHashList[randomIndex];

	// Remover o modelo da mem�ria para evitar duplica��o de cor/ra�a
	Hash horseModel = MISC::GET_HASH_KEY(selectedHorseHash.c_str());
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(horseModel);

	// Mover o cavalo selecionado para a lista de usados
	usedHorseHashList.push_back(selectedHorseHash);

	// Remover o cavalo selecionado da lista para n�o gerar o mesmo
	availableHorseHashList.erase(availableHorseHashList.begin() + randomIndex);

	// Retornar o hash do cavalo
	return selectedHorseHash;
}

// Fun��o para gerar coordenadas opostas � posi��o atual do jogador
std::pair<std::string, Vector3> GetRandomCoord() {
	// Semente para o gerador de n�meros aleat�rios
	std::srand(static_cast<unsigned int>(std::time(0)));

	// Verificar se ainda h� coordenadas dispon�veis
	if (availableCoordList.empty()) {
		// Reiniciar a lista se todas as coordenadas j� foram usadas
		availableCoordList = coordenadas;
		usedCoordList.clear(); // Limpa as coordenadas usadas
	}

	// ID do jogador e coordenadas atuais
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);

	// Vari�veis para armazenar a coordenada mais distante
	std::pair<std::string, Vector3> farthestCoord;
	float maxDistance = -1.0f;

	// Encontrar a coordenada mais distante do jogador
	for (const auto& coord : availableCoordList) {
		float distance = MISC::GET_DISTANCE_BETWEEN_COORDS(
			playerPos.x, playerPos.y, playerPos.z,
			coord.second.x, coord.second.y, coord.second.z, true);

		// Se a dist�ncia for maior que a m�xima encontrada, atualizar
		if (distance > maxDistance) {
			maxDistance = distance;
			farthestCoord = coord;
		}
	}

	// Mover a coordenada selecionada para a lista de usadas
	usedCoordList.push_back(farthestCoord);

	// Encontrar e remover a coordenada mais distante da lista dispon�vel
	for (auto it = availableCoordList.begin(); it != availableCoordList.end(); ++it) {
		if (it->first == farthestCoord.first &&
			it->second.x == farthestCoord.second.x &&
			it->second.y == farthestCoord.second.y &&
			it->second.z == farthestCoord.second.z) {
			availableCoordList.erase(it);
			break;
		}
	}

	// Retornar a coordenada mais distante encontrada
	return farthestCoord;
}

/*
// Fun��o para gerar coordenadas aleat�rias e remover ap�s o uso
std::pair<std::string, Vector3> GetRandomCoord() {
	// Semente para o gerador de n�meros aleat�rios
	std::srand(static_cast<unsigned int>(std::time(0)));

	// Verificar se ainda h� coordenadas dispon�veis
	if (availableCoordList.empty()) {
		// Reiniciar a lista se todas as coordenadas j� foram usadas
		availableCoordList = coordenadas;
		usedCoordList.clear(); // Limpa as coordenadas usadas
	}

	// Obter um �ndice aleat�rio
	int randomIndex = std::rand() % availableCoordList.size();

	// Obter a coordenada correspondente ao �ndice aleat�rio
	std::pair<std::string, Vector3> selectedCoord = availableCoordList[randomIndex];

	// Mover a coordenada selecionada para a lista de usadas
	usedCoordList.push_back(selectedCoord);

	// Remover a coordenada da lista dispon�vel
	availableCoordList.erase(availableCoordList.begin() + randomIndex);

	// Retornar a coordenada selecionada
	return selectedCoord;
}
*/

bool isPlayerWithinDistance(double maxDistance) {
	// Obter a entidade do jogador (Ped)
	Ped player_ped = PLAYER::PLAYER_PED_ID();

	// Obter as coordenadas atuais do jogador
	Vector3 player_coords = ENTITY::GET_ENTITY_COORDS(player_ped, true, true);

	// Converter as coordenadas do jogador para double
	double playerX = static_cast<double>(player_coords.x);
	double playerY = static_cast<double>(player_coords.y);
	double playerZ = static_cast<double>(player_coords.z);

	// Iterar sobre cada local na lista e verificar a dist�ncia
	for (const auto& location : locations) {
		double locX = location.coords.x;
		double locY = location.coords.y;
		double locZ = location.coords.z;

		// Calcular a dist�ncia entre o jogador e o local usando VDIST
		double distance = BUILTIN::VDIST((float)playerX, (float)playerY, (float)playerZ, (float)locX, (float)locY, (float)locZ);

		// Verificar se a dist�ncia � menor ou igual � dist�ncia m�xima
		if (distance <= maxDistance) {
			logMessage("Player is near " + location.name + ". Distance: " + std::to_string(distance));
			return true;
		}
	}

	// Se n�o estiver perto de nenhuma localiza��o, retornar false
	return false;
}
