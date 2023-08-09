#pragma once

#include <cstdio>

//printf("%s:%d %s " format "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define LOG(format, ...) \
    do { \
        printf("%s:%d " format "\n", __func__, __LINE__, ##__VA_ARGS__); \
        fflush(stdout); \
    } while(0)


