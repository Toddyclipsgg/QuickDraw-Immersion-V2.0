#include "..\header.h"
#include "gamefunctions.h"
#include "menuconstructor.h"

// Variáveis globais para armazenar a hora, minuto e segundo
int selectedHour = 0;   // Hora inicial
int selectedMinute = 0;  // Minuto inicial
int selectedSecond = 0;  // Segundo inicial
bool bPauseTime = false; // Para pausar ou despausar o tempo
bool isTimePaused = false; // Inicia com false, pois o tempo não está pausado inicialmente

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
        addHeader("Follow me @toddyclipsgg");
        // Adiciona opção para acessar o Submenu 1
        addSubmenuOption("Gun Tricks", Submenu1, [] { PrintSubtitle("Select your Gun Tricks.\nFavorite whenever you want, whenever you want!"); });
        // Adiciona opção para acessar o Submenu 2
        addSubmenuOption("Change Weather", Submenu2, [] { PrintSubtitle("Function for cinematic purposes only."); });
        // Adiciona opção para acessar o Submenu 3
        addSubmenuOption("Change Time", Submenu3, [] { PrintSubtitle("Function for cinematic purposes only."); });
        // Adiciona opção para acessar o Submenu 3
        // addSubmenuOption("About QuickDraw", UpdateText, [] { PrintSubtitle("Mod Updates Summary!"); });
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
        break;

    case Submenu1:
        // Define o título do Submenu 1
        addTitle("TRICKS");
        addHeader("Follow me @toddyclipsgg");
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
        break;

    case Submenu2:
        // Define o título do Submenu 2
        addTitle("WEATHER");
        addHeader("Follow me @toddyclipsgg");
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
        break;

    case Submenu3:
        addTitle("TIME");
        Update();
        addVectorOption("Hour", &selectedHour, 1, 0, 23); // Seleção de hora
        addVectorOption("Minute", &selectedMinute, 1, 0, 59); // Seleção de minuto
        addVectorOption("Second", &selectedSecond, 1, 0, 59); // Seleção de segundo
        addBoolOption("Pause Time", bPauseTime, PauseTime); // Pausa o tempo
        break;

    case UpdateText:
        addTitle("UPDATE");
        addHeader("Follow me @toddyclipsgg");
        addUpdate("V2.0 QuickDraw from 0.4 ms to 0.2 ms.");
        addUpdate("V2.0 Gun Tricks from 0.9 ms to 0.5 ms.");
        addUpdate("V2.0 Added the O'Driscolls");
        break;
    }
    resetVars(); // Reseta variáveis do menu
}
