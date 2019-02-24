// DonkeyKong.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Game.h"
#include "StartScreen.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include<ctime>
#include<cstdlib>
#endif

int main() {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    srand (time(NULL));
    #endif
    StartScreen startScreen;
    startScreen.run();
}
