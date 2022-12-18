#pragma once

#include <stdio.h>
#include <type_traits>
#include <initializer_list>
#include <type_traits>
#include <typeinfo>

// #define CREATE_CLASS(setup, teardown)                        \
//     class A {                                                              \
//     public:                                                                \
//         void SetUp() {                                                     \
//             if (std::is_invocable<void ()>(setup)) {                       \
//                 setup();                                                   \
//             } else {                                                       \
//                 printf("%u\n", setup);                                     \
//             }                                                              \
//         }                                                                  \
//                                                                            \
//         void TearDown() {                                         \
//             teardown();                                                    \
//         }                                                                  \
//     };


typedef enum {
    SLevel_A,
    SLevel_B,
    SLevelC
} SLevel;

#define SETUPS(...) {__VA_ARGS__}

//    static_assert(sizeof(#fixture_name) > 1, "fixture name must be at least 1 char"); \
//     static_assert(setups.size() > 1, "must contain at least 1 setup function"); \

// static_assert(std::is_same<decltype(setups), std::initializer_list>::value, "dv");
// static_assert(std::is_same<decltype(setups), std::initializer_list<void (*)()>>::value, "dv");

#define CREATE_CLASS(fixture_name, level, setups, teardown)                \
    static_assert(sizeof(#fixture_name) > 0, "fixture name can't be empty"); \
    static_assert(std::is_same<decltype(level), SLevel>::value, "level must be SLevel"); \
    class fixture_name {                                                   \
    public:                                                                \
        void SetUp() {                                                     \
            for (const auto& setup: setups) {                              \
                setup();                                                   \
            }                                                              \
        }                                                                  \
                                                                           \
        void TearDown() {                                                  \
            teardown();                                                    \
        }                                                                  \
    };


// template<class T>
// class GTestWrapper {
// public:
//     GTestWrapper(auto fixture_name, auto )

// };


static auto no_setup = [](){};
static auto no_teardown = [](){};
