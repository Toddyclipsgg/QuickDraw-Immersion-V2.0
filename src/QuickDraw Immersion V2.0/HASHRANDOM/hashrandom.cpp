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

// Função para gerar coordenadas aleatórias e remover após o uso
std::pair<std::string, Vector3> GetRandomCoord() {
	// Semente para o gerador de números aleatórios
	std::srand(static_cast<unsigned int>(std::time(0)));

	// Verificar se ainda há coordenadas disponíveis
	if (availableCoordList.empty()) {
		// Reiniciar a lista se todas as coordenadas já foram usadas
		availableCoordList = coordenadas;
		usedCoordList.clear(); // Limpa as coordenadas usadas
	}

	// Obter um índice aleatório
	int randomIndex = std::rand() % availableCoordList.size();

	// Obter a coordenada correspondente ao índice aleatório
	std::pair<std::string, Vector3> selectedCoord = availableCoordList[randomIndex];

	// Mover a coordenada selecionada para a lista de usadas
	usedCoordList.push_back(selectedCoord);

	// Remover a coordenada da lista disponível
	availableCoordList.erase(availableCoordList.begin() + randomIndex);

	// Retornar a coordenada selecionada
	return selectedCoord;
}