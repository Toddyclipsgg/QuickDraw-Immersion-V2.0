#pragma once

#ifndef HEADER_H
#define HEADER_H
// Natives
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"
#include "..\..\inc\main.h"

// #include "header.h"

// Utils
#include "logger.h"

// Scripts
#include "script.h"
#include "keyboard.h"
#include "utils.h"

// Windows Header
#include "../../inc/MinHook/MinHook.h"
#pragma comment(lib, "../../inc/MinHook/libMinHook.x64.lib")
#include <string>
#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
#include <functional>
#include <map>

extern std::vector<Entity> globalEntityList;
extern std::vector<Entity> vehicleEntityList;
extern int selected_single_variation;
extern std::vector<Ped> pedList;
extern std::map<Ped, Blip> pedBlips;

// Logger
#include "logger.h"

// Mod Header
#include "CREATORPED\creatorped.h"
#include "CREATORPED\setpresetped.h"
#include "SPAWNPED\spawnped.h"
#include "HASHRANDOM\hashrandom.h"
#include "GANG\spawngroupmanager.h"
#include "GANG\odriscollsspawn.h"
#include "GANG\ambush.h"
#include "MOD\quickdraw.h"
#include "MOD\setaccuracy.h"
#include "MOD\twirl.h"
#include "BLIP\blips.h"
#include "CREATORVEHICLE\spawnvehicle.h"
#include "MENU\menu_flutter.h"

#endif // HEADER_H