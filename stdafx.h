// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

//std
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <random>

//SDL
#include "SDL.h"

//Win
#include<Windows.h>
#include<Kinect.h>

//safely resource release mechanism
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}