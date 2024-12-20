/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#pragma once

#include "main.h"

template <typename T>
static inline void nativePush(T val)
{
	UINT64 val64 = 0;
	if (sizeof(T) > sizeof(UINT64))
	{
		throw "error, value size > 64 bit";
	}
	*reinterpret_cast<T *>(&val64) = val; // &val + sizeof(dw) - sizeof(val)
	nativePush64(val64);
}

static inline void pushArgs()
{
}

template <typename T>
static inline void pushArgs(T arg)
{
	nativePush(arg);
}

template <typename T, typename... Ts>
static inline void pushArgs(T arg, Ts... args)
{
	nativePush(arg);
	pushArgs(args...);
}

template <typename R, typename... Ts>
static inline R invoke(UINT64 hash, Ts... args)
{
	nativeInit(hash);
	pushArgs(args...);

	void* result = nativeCall(); // Recebe o resultado como um ponteiro gen�rico

	// Verifica se o ponteiro � v�lido antes de desreferenciar
	if (!result)
	{
		throw "nativeCall returned a null pointer";
	}

	// Trata o ponteiro como o tipo R e desreferencia
	R* typedResult = reinterpret_cast<R*>(result);

	// Se R � um tipo fundamental, como int ou float, voc� pode retornar diretamente
	// Se for um tipo mais complexo, considere uma abordagem diferente
	return *typedResult;
}
