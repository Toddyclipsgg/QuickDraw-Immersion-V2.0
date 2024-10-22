#include "..\header.h"
#include "gamefunctions.h"
#include "menuconstructor.h"

// Define membro plus/free
#define MEMBER_PLUS 0
#define MEMBER_FREE 1

// Variáveis globais para armazenar a hora, minuto e segundo
int selectedHour = 0;   // Hora inicial
int selectedMinute = 0;  // Minuto inicial
int selectedSecond = 0;  // Segundo inicial
bool bPauseTime = false; // Para pausar ou despausar o tempo
bool isTimePaused = false; // Inicia com false, pois o tempo não está pausado inicialmente

void OpenWebsite() {
    const char* url = "https://www.patreon.com/c/toddyclipsgg/membership"; // Defina aqui o site que você quer abrir
    ShellExecute(0, "open", url, NULL, NULL, SW_SHOWNORMAL); // Abre o navegador no site
}

// Função para obter a hora atual e atualizar as variáveis
void UpdateCurrentTime() {
    selectedHour = CLOCK::GET_CLOCK_HOURS();
    selectedMinute = CLOCK::GET_CLOCK_MINUTES();
    selectedSecond = CLOCK::GET_CLOCK_SECONDS();
}

// Função para exibir a hora atual no menu
void UpdateTimeDisplay() {
    // Formatar a string de exibição da hora
    char timeMessage[128];
    snprintf(timeMessage, sizeof(timeMessage), "Current Time: %02d:%02d:%02d", selectedHour, selectedMinute, selectedSecond);
    PrintSubtitle(timeMessage);
    addHeader(timeMessage); // Adiciona a hora ao cabeçalho
}

// Função de atualização chamada em um loop, com base na variável isTimePaused
void Update() {
    // Exibir a hora atual, independente da pausa
    UpdateTimeDisplay(); // Atualiza a exibição de tempo em tempo real

    // Atualiza a hora apenas se o tempo não estiver pausado
    if (!isTimePaused) {
        UpdateCurrentTime();
    }
}

// Função para pausar e despausar o tempo (e atualizar isTimePaused)
void PauseTime() {
    // Alterna o estado de bPauseTime e aplica ao relógio
    bPauseTime = !bPauseTime;
    CLOCK::PAUSE_CLOCK(bPauseTime, 0);

    // Atualiza a variável isTimePaused para informar a função Update
    isTimePaused = bPauseTime;

    PrintSubtitle(bPauseTime ? "Time paused" : "Time unpaused");

    // Atualiza a hora apenas ao despausar
    if (!bPauseTime) {
        UpdateCurrentTime();
    }
}

// Função que chama para ajustar a hora
void AdjustTime() {
    CLOCK::SET_CLOCK_TIME(selectedHour, selectedMinute, selectedSecond);
}

void SetWeather(Hash weatherType, const char* weatherName)
{
    MISC::SET_WEATHER_TYPE(weatherType, true, true, false, 0.0f, false);

    // Concatena as strings e usa .c_str() para converter std::string para const char*
    std::string message = "Set weather type to ~COLOR_PAUSE_NAV_BG~" + std::string(weatherName) + "~s~";

    PrintSubtitle(message.c_str());  // Usa .c_str() para converter std::string para const char*
}

void SetGunTrickAnimation(int selected_variation)
{
    const char* animationName;

    // Associa o número da variação ao nome da animação
    switch (selected_variation)
    {
    case 0:
        animationName = "Reverse Spin";
        break;
    case 1:
        animationName = "Spin UP";
        break;
    case 2:
        animationName = "Reverse Spin UP";
        break;
    case 3:
        animationName = "Alternating Flips";
        break;
    case 4:
        animationName = "Shoulder Toss";
        break;
    case 5:
        animationName = "Figure Eight Toss";
        break;
    default:
        animationName = "Unknown Trick";
        break;
    }

    // Exibe o nome da animação
    char message[128];
    snprintf(message, sizeof(message), "Selected Gun Trick: ~COLOR_PAUSE_NAV_BG~%s~s~", animationName);
    PrintSubtitle(message);
}

// Variável para controlar a opção selecionada de string
int stringoptionint = 0;

// Array com 6 strings para serem usadas nas opções do menu
const char* strings[6] = {
    "String_1", "String_2", "String_3", "String_4", "String_5", "String_6"
};

// Loop do menu | Tudo é chamado dentro desta função
void startmenu()
{
    ButtonMonitoring(); // Monitora o pressionamento de botões
    switch (submenu)
    {
    case Main_Menu:
        // Define o título e cabeçalho do menu principal
        addTitle("QUICKDRAW");
        addHeader("Mod by Toddyclipsgg.");
#if MEMBER_PLUS
        addOptionPulse("Discover more mods for RDR2 and GTA 6.", OpenWebsite);
#endif // MEMBER_PLUS

#if MEMBER_FREE
        addOptionPulse("More Features, More Power. Click to Unlock!", OpenWebsite);
#endif // MEMBER_FREE

#if MEMBER_PLUS
        // Adiciona opção para acessar o Gun Tricks Plus 
        addSubmenuOption("Gun Tricks", SubmenuPlusTricks, [] { PrintSubtitle("Select your Gun Tricks.\nFavorite whenever you want, whenever you want!"); });
#endif // MEMBER_PLUS

#if MEMBER_FREE
        // Adiciona opção para acessar o Gun Tricks Free
        addSubmenuOption("Gun Tricks", SubmenuFreeTricks, [] { PrintSubtitle("Select your Gun Tricks.\nFavorite whenever you want, whenever you want!"); });
#endif // MEMBER_FREE

#if MEMBER_PLUS
        // Adiciona opção para acessar o Weather Plus
        addSubmenuOption("Change Weather", SubmenuPlusWeather, [] { PrintSubtitle("Function for cinematic purposes only."); });
#endif // MEMBER_PLUS

#if MEMBER_FREE
        // Adiciona opção para acessar o Weather Free
        addSubmenuOption("Change Weather", SubmenuFreeWeather, [] { PrintSubtitle("Function for cinematic purposes only."); });
#endif // MEMBER_FREE

#if MEMBER_PLUS
        // Adiciona opção para acessar o Time Plus
        addSubmenuOption("Change Time", SubmenuPlusTime, [] { PrintSubtitle("Function for cinematic purposes only."); });
#endif // MEMBER_PLUS

#if MEMBER_FREE
        // Adiciona opção para acessar o Time Free
        addSubmenuOption("Change Time", SubmenuFreeTime, [] { PrintSubtitle("Function for cinematic purposes only."); });
#endif // MEMBER_FREE

        // Adiciona opção para acessar o Submenu 4
        addSubmenuOption("Mod updates", Updates, [] { PrintSubtitle("Mod Updates Summary!"); });
        // Adiciona opção para ativar/desativar o modo Deus
        // addBoolOption("Godmode", Godmode, [] {Godmode = !Godmode; });
        // Adiciona opção para ativar/desativar a invisibilidade do jogador
        // addBoolOption("Invisibility", !ENTITY::IS_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID()), [] {ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), !ENTITY::IS_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID())); });
        // Adiciona opção para alterar a escala do jogador
        // addFloatOption("Set Player Scale", &playerScale, 0.1f, SetPlayerScale);
        // Adiciona opção para teleportar o jogador para frente
        // addOption("Teleport Forward", TeleportForward);
        // Adiciona opção de string, que altera entre as strings disponíveis no array
        // addStringOption("Strings", strings[stringoptionint], &stringoptionint, ARRAYSIZE(strings) - 1, [] {PrintSubtitle(strings[stringoptionint]); });
        PrintSubtitle("To exit the menu press Backspace.");
        break;

    case SubmenuPlusTricks:
        // Define o título do Submenu 1
        addTitle("GUN TRICKS");
        addHeader("Mod by Toddyclipsgg.");
        addOption("Reverse Spin", [] {
            selected_single_variation = 0;
            SetGunTrickAnimation(selected_single_variation);
            });
        addOption("Spin UP", [] {
            selected_single_variation = 1;
            SetGunTrickAnimation(selected_single_variation);
            });
        addOption("Reverse Spin UP", [] {
            selected_single_variation = 2;
            SetGunTrickAnimation(selected_single_variation);
            });
        addOption("Alternating Flips", [] {
            selected_single_variation = 3;
            SetGunTrickAnimation(selected_single_variation);
            });
        addOption("Shoulder Toss", [] {
            selected_single_variation = 4;
            SetGunTrickAnimation(selected_single_variation);
            });
        addOption("Figure Eight Toss", [] {
            selected_single_variation = 5;
            SetGunTrickAnimation(selected_single_variation);
            });
        addOptionPulse("More Features, More Power. Click to Unlock!", OpenWebsite);
        break;

    case SubmenuFreeTricks:
        // Define o título do Submenu 1
        addTitle("GUN TRICKS");
        addHeader("Mod by Toddyclipsgg.");
        addOption("Reverse Spin", [] {
            selected_single_variation = 0;
            SetGunTrickAnimation(selected_single_variation);
            });
        addOption("Spin UP", [] {
            selected_single_variation = 1;
            SetGunTrickAnimation(selected_single_variation);
            });
        addOptionPulse("Unlock +4 animations. Click to Unlock!", OpenWebsite);
        break;

    case SubmenuPlusWeather:
        // Define o título do Submenu 2
        addTitle("CHANGE WEATHER");
        addHeader("Mod by Toddyclipsgg.");
        addOption("High Pressure", [] { SetWeather(MISC::GET_HASH_KEY("HIGHPRESSURE"), "High Pressure"); });
        addOption("Rain", [] { SetWeather(MISC::GET_HASH_KEY("RAIN"), "Rain"); });
        addOption("Snow", [] { SetWeather(MISC::GET_HASH_KEY("SNOW"), "Snow"); });
        addOption("Misty", [] { SetWeather(MISC::GET_HASH_KEY("MISTY"), "Misty"); });
        addOption("Fog", [] { SetWeather(MISC::GET_HASH_KEY("FOG"), "Fog"); });
        addOption("Sunny", [] { SetWeather(MISC::GET_HASH_KEY("SUNNY"), "Sunny"); });
        addOption("Clouds", [] { SetWeather(MISC::GET_HASH_KEY("CLOUDS"), "Clouds"); });
        addOption("Overcast", [] { SetWeather(MISC::GET_HASH_KEY("OVERCAST"), "Overcast"); });
        addOption("Thunderstorm", [] { SetWeather(MISC::GET_HASH_KEY("THUNDERSTORM"), "Thunderstorm"); });
        addOption("Hurricane", [] { SetWeather(MISC::GET_HASH_KEY("HURRICANE"), "Hurricane"); });
        addOption("Thunder", [] { SetWeather(MISC::GET_HASH_KEY("THUNDER"), "Thunder"); });
        addOption("Shower", [] { SetWeather(MISC::GET_HASH_KEY("SHOWER"), "Shower"); });
        addOption("Blizzard", [] { SetWeather(MISC::GET_HASH_KEY("BLIZZARD"), "Blizzard"); });
        addOption("Snow Light", [] { SetWeather(MISC::GET_HASH_KEY("SNOWLIGHT"), "Snow Light"); });
        addOption("Whiteout", [] { SetWeather(MISC::GET_HASH_KEY("WHITEOUT"), "Whiteout"); });
        addOption("Hail", [] { SetWeather(MISC::GET_HASH_KEY("HAIL"), "Hail"); });
        addOption("Sleet", [] { SetWeather(MISC::GET_HASH_KEY("SLEET"), "Sleet"); });
        addOption("Drizzle", [] { SetWeather(MISC::GET_HASH_KEY("DRIZZLE"), "Drizzle"); });
        addOption("Sandstorm", [] { SetWeather(MISC::GET_HASH_KEY("SANDSTORM"), "Sandstorm"); });
        addOption("Overcast Dark", [] { SetWeather(MISC::GET_HASH_KEY("OVERCASTDARK"), "Overcast Dark"); });
        addOption("Ground Blizzard", [] { SetWeather(MISC::GET_HASH_KEY("GROUNDBLIZZARD"), "Ground Blizzard"); });
        addOptionPulse("More Features, More Power. Click to Unlock!", OpenWebsite);
        break;

    case SubmenuFreeWeather:
        // Define o título do Submenu 2
        addTitle("CHANGE WEATHER");
        addHeader("Mod by Toddyclipsgg.");
        addOption("High Pressure", [] { SetWeather(MISC::GET_HASH_KEY("HIGHPRESSURE"), "High Pressure"); });
        addOption("Rain", [] { SetWeather(MISC::GET_HASH_KEY("RAIN"), "Rain"); });
        addOption("Snow", [] { SetWeather(MISC::GET_HASH_KEY("SNOW"), "Snow"); });
        addOption("Misty", [] { SetWeather(MISC::GET_HASH_KEY("MISTY"), "Misty"); });
        addOption("Fog", [] { SetWeather(MISC::GET_HASH_KEY("FOG"), "Fog"); });
        addOptionPulse("Desbloqueie + 16 weather. Click to Unlock!", OpenWebsite);
        break;

    case SubmenuPlusTime:
        addTitle("CHANGE TIME");
        Update();
        addVectorOption("Hour", &selectedHour, 1, 0, 23); // Seleção de hora
        addVectorOption("Minute", &selectedMinute, 1, 0, 59); // Seleção de minuto
        addVectorOption("Second", &selectedSecond, 1, 0, 59); // Seleção de segundo
        addBoolOption("Pause Time", bPauseTime, PauseTime); // Pausa o tempo
        addOptionPulse("More Features, More Power. Click to Unlock!", OpenWebsite);
        break;

    case SubmenuFreeTime:
        addTitle("CHANGE TIME");
        Update();
        addVectorOption("Hour", &selectedHour, 1, 0, 23); // Seleção de hora
        addVectorOption("Minute", &selectedMinute, 1, 0, 59); // Seleção de minuto
        addVectorOption("Second", &selectedSecond, 1, 0, 59); // Seleção de segundo
        addOptionPulse("Unlock pause time. Click to Unlock!", OpenWebsite);
        break;

    case Updates:
        addTitle("UPDATES");
        addHeader("Mod by Toddyclipsgg.");
        addUpdate("Update Quickdraw 4ms to 2ms.");
        addUpdate("Update Gun Tricks 9ms to 5ms.");
        addUpdate("Simpler and more intuitive menu.");
        addUpdate("Add mounted gang O'driscoll.");
        addOptionPulse("More Features, More Power. Click to Unlock!", OpenWebsite);
        break;
    }
    resetVars(); // Reseta variáveis do menu
}

void printStartMod() {

    PrintSubtitle("Press F7 and see more mods to download.\nTo exit the menu press Backspace.");

}

// Função para obter a direção cardinal com base no heading do jogador
std::string getCardinalDirection(double heading) {
    if (heading >= 315.0 || heading < 45.0) {
        return "N";
    }
    else if (heading >= 45.0 && heading < 135.0) {
        return "E";
    }
    else if (heading >= 135.0 && heading < 225.0) {
        return "S";
    }
    else {
        return "W";
    }
}

// Função principal para exibir o painel com as coordenadas e o heading do jogador
void DisplayPlayerCoordinatesPanel() {
    // Obter a entidade do jogador (Ped)
    Ped player_ped = PLAYER::PLAYER_PED_ID();

    // Obter as coordenadas e a orientação (heading) do jogador
    Vector3 player_coords = ENTITY::GET_ENTITY_COORDS(player_ped, true, true);
    float player_heading = ENTITY::GET_ENTITY_HEADING(player_ped);

    // Converter as coordenadas e o heading para double
    double player_coords_x = static_cast<double>(player_coords.x);
    double player_coords_y = static_cast<double>(player_coords.y);
    double player_coords_z = static_cast<double>(player_coords.z);
    double player_heading_d = static_cast<double>(player_heading);

    // Converter as coordenadas e o heading para strings formatadas
    char coordsText[256];
    sprintf_s(coordsText, "X: %.6lf | Y: %.6lf | Z: %.6lf", player_coords_x, player_coords_y, player_coords_z);
    char headingText[64];
    sprintf_s(headingText, "%.2lf (%s)", player_heading_d, getCardinalDirection(player_heading_d).c_str());

    // Posição e dimensões do painel
    float panelX = 0.022f;
    float panelY = 0.030f;
    float panelWidth = 0.300f;
    float panelHeight = 0.038f; // Altura ajustada para acomodar as informações

    // Desenhar o fundo do painel (cor preta semi-transparente)
    drawRect(panelX, panelY, panelWidth, panelHeight, 66, 66, 66, 190);

    // Desenhar as coordenadas do jogador
    draw_Text(coordsText, panelX + 0.007f, panelY + 0.007f, 255, 255, 255, 255);

    // Desenhar a orientação (heading) do jogador
    draw_Text(headingText, panelX + 0.24f, panelY + 0.007f, 255, 255, 255, 255);
}
