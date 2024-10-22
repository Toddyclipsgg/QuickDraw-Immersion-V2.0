// logger.h
#ifndef LOGGER_H
#define LOGGER_H

// Defina esta macro para habilitar o console
#define CONSOLE_ENABLED 0
#define INJECTION_CONSOLE_ENABLE 0

#include <fstream>
#include <thread>
#include <mutex>

extern std::ofstream logFile;
extern std::thread logThread;
extern std::mutex logMutex;
extern bool isLoggingEnabled;
extern bool isConsoleEnabled;

void initLog();
void closeLog();
void logToFile(const std::string& message);
void logToConsole(const std::string& message);
void logMessage(const std::string& message);
void keyboardHandler();
void startLogThread();
void stopLogThread();

#endif // LOGGER_H
