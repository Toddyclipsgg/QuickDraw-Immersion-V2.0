#include "header.h"

// Declaração de variáveis globais para log e console
std::ofstream logFile;
bool isLoggingEnabled = true;
bool isConsoleEnabled = true; // Ative/Desative tambem a INJECTION_CONSOLE_ENABLE em (logger.h)
std::mutex logMutex;
std::thread logThread;
std::chrono::steady_clock::time_point lastLogTime = std::chrono::steady_clock::now();
const std::chrono::seconds logInterval(0); // Intervalo entre mensagens de log

// Inicializa o arquivo de log
void initLog() {
    logFile.open("QuickDraw_Immersion_V2.0_Log.txt", std::ofstream::out | std::ofstream::trunc); // Sobrescreve o log anterior
    if (logFile.is_open()) {
        logFile << "Iniciando log do mod...\n";
#if CONSOLE_ENABLED
        std::cout << "Log inicializado\n";
#endif
    }
}

// Fecha o arquivo de log
void closeLog() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

// Função para logar informações no arquivo
void logToFile(const std::string& message) {
    std::lock_guard<std::mutex> guard(logMutex); // Garante que as threads não colidam
    if (isLoggingEnabled && logFile.is_open()) {
        logFile << message << std::endl;
    }
}

// Função para exibir no console
void logToConsole(const std::string& message) {
#if CONSOLE_ENABLED
    if (isConsoleEnabled) {
        std::cout << message << std::endl;
    }
#endif
}

// Função para controlar o log e console
void logMessage(const std::string& message) {
    auto now = std::chrono::steady_clock::now();
    if (now - lastLogTime >= logInterval) {
        logToConsole(message);  // Exibe no console
        logToFile(message);     // Escreve no arquivo
        lastLogTime = now;
    }
}

// Função para logar imediatamente
void logImmediate(const std::string& message) {
    logToConsole(message);  // Exibe no console
    logToFile(message);     // Escreve no arquivo
}

// Controle de teclado para ativar/desativar log e console
void keyboardHandler() {
    if (IsKeyJustUp(VK_F6)) {
        isLoggingEnabled = !isLoggingEnabled;
        logImmediate("Logging foi " + std::string(isLoggingEnabled ? "ativado" : "desativado"));
    }

    if (IsKeyJustUp(VK_F7)) {
        isConsoleEnabled = !isConsoleEnabled;
        logImmediate("Console foi " + std::string(isConsoleEnabled ? "ativado" : "desativado"));
    }
}

// Função para rodar o logger em uma thread separada
void startLogThread() {
    logThread = std::thread([]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Verifica a cada segundo
            if (!isLoggingEnabled) break; // Para a thread se o logging estiver desativado
            // Aqui você pode adicionar outras operações a serem feitas periodicamente
        }
        });
}

// Função para parar a thread de log
void stopLogThread() {
    if (logThread.joinable()) {
        isLoggingEnabled = false; // Sinaliza para parar a thread
        logThread.join(); // Espera a thread terminar
    }
}
