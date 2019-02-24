#pragma once
#include <random>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#endif


static int getRandomNumber(int start, int end) {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    return rand() % ((end + 1) - start) + start;
    #else
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<int> distr(start, end);
    return distr(eng);
    #endif
}
