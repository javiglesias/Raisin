#pragma once

#ifdef _WINDOWS
	#ifdef __RAISIN_DLL_BUILD
		#define RAISIN_API __declspec(dllexport)
	#else
		#define RAISIN_API __declspec(dllimport)	
	#endif
#else
	#error "ONLY WINDOWS"
#endif

#include <cstdio>
#include "Vector.h"
#include "Input/Input.h"

#define PI 3.14
#define PIPI 2*3.14

