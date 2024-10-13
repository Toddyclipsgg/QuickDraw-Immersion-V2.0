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