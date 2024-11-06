/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2015
*/

#pragma once

#include <windows.h>

typedef DWORD Void;
typedef DWORD Any;
typedef DWORD uint;
typedef DWORD Hash;
typedef int Entity;
typedef int Player;
typedef int FireId;
typedef int Ped;
typedef int Vehicle;
typedef int Cam;
typedef int CarGenerator;
typedef int Group;
typedef int Train;
typedef int Pickup;
typedef int Object;
typedef int Weapon;
typedef int Interior;
typedef int Blip;
typedef int Texture;
typedef int TextureDict;
typedef int CoverPoint;
typedef int Camera;
typedef int TaskSequence;
typedef int ColourIndex;
typedef int Sphere;
typedef int ScrHandle;
typedef int Prompt;
typedef int Volume;
typedef int PropSet;
typedef int PopZone;
typedef int PersChar;
typedef int ItemSet;
typedef int AnimScene;

#define ALIGN8 __declspec(align(8))

struct Vector3
{
    ALIGN8 float x;
    ALIGN8 float y;
    ALIGN8 float z;

    // Construtor que aceita três argumentos float
    Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

    bool isZero() const {
        return x == 0.0f && y == 0.0f && z == 0.0f;
    }
};

static_assert(sizeof(Vector3) == 24, "");
