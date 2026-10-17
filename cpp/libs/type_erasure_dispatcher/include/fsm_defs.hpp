#pragma once

#include <cstdint>

enum class FsmState : uint8_t {
    A = 1,
    B,
    C,
};

const char* fsm_state_to_str(FsmState state) {
    switch(state) {
        case FsmState::A: {
            return "A";
        }
        case FsmState::B: {
            return "B";
        }
        case FsmState::C: {
            return "C";
        }
    }

    return "N/A";
}

struct X {};
struct Y {};
struct Z {
    int z;
};

