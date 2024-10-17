#include "..\header.h"

// Fun��o para obter uma posi��o aleat�ria dentro de um raio a partir de um centro
Vector3 getRandomPositionInRange(Vector3 center, int radius)
{

    // Gera��o de coordenadas aleat�rias dentro do raio
    int x = rndInt((int)center.x - radius, (int)center.x + radius + 1);
    int topOrBottom = rndInt(0, 2) == 1 ? 1 : -1;
    double y = topOrBottom * sqrt(pow(radius, 2) - pow(x - center.x, 2)) + center.y;

    // Definindo a sa�da com a posi��o calculada
    Vector3 output;
    output.x = (float)x;
    output.y = (float)y;
    output.z = getGroundPos(output); // Obtendo a posi��o do ch�o
    logMessage("Random position calculated successfully.");

    return output;
}

// Fun��o que retorna apenas o valor Z do solo
float getGroundPos(Vector3 originalPos)
{

    float groundZ;
    MISC::GET_GROUND_Z_FOR_3D_COORD(originalPos.x, originalPos.y, originalPos.z + 30, &groundZ, false);

    logMessage("Ground position successfully obtained.");
    return groundZ;
}

// Fun��o para obter uma posi��o segura para um ped dentro de um raio
Vector3 getRandomPedPositionInRange(Vector3 source, int radius)
{

    // Obtendo uma posi��o aleat�ria
    Vector3 position = getRandomPositionInRange(source, radius);

    // Verifica��o de seguran�a para coordenadas de pedestre
    PATHFIND::GET_SAFE_COORD_FOR_PED(position.x, position.y, position.z, true, &position, 16);

    logMessage("Safe pedestrian position successfully achieved.");

    return position;
}

// Fun��o r�pida para gerar um n�mero inteiro aleat�rio entre 'start' e 'end'
int rndInt(int start, int end) {
    logMessage("Generating random integer.");
    return MISC::GET_RANDOM_INT_IN_RANGE(start, end);
}

//fun��o r�pida para obter a dist�ncia entre 2 pontos: por exemplo - if (distanceBetween(coordsOf(player), targetCoords) < 50)
float distanceBetween(Vector3 A, Vector3 B) {
    return MISC::GET_DISTANCE_BETWEEN_COORDS(A.x, A.y, A.z, B.x, B.y, B.z, 1);
}