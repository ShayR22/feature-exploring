#pragma once

#include "fsm_defs.hpp"
#include "fsm_dispatcher.hpp"


class Fsm {
public:
    Fsm() {
        REGISTER_TRANSITION(_dispatcher, FsmState::A, X, x_from_a);
        REGISTER_TRANSITION(_dispatcher, FsmState::B, X, x_from_b);
        REGISTER_TRANSITION(_dispatcher, FsmState::B, Y, y_from_b);
        REGISTER_TRANSITION(_dispatcher, FsmState::C, Z, z_from_c);
        _dispatcher.print_transition_input_to_cb();
    }

    void x(const X& x) {
        _dispatcher.transition(x);
    }

    void y(const Y& y) {
        _dispatcher.transition(y);
    }

    void z(const Z& z) {
        _dispatcher.transition(z);
    }

private:
    FsmDispatcher _dispatcher{};

    void x_from_a(FsmState& new_state, const X&) {
        printf("Enter %s\n", __func__);
        new_state = FsmState::B;
    }

    void x_from_b(FsmState& new_state, const X&) {
        printf("Enter %s\n", __func__);
        new_state = FsmState::A;
    }

    void y_from_b(FsmState& new_state, const Y&) {
        printf("Enter %s\n", __func__);
        new_state = FsmState::C;
    }

    void z_from_c(FsmState& new_state, const Z& z) {
        printf("Enter %s\n", __func__);
        if (z.z == 1) {
            new_state = FsmState::B;
        } else {
            new_state = FsmState::A;
        }
    }
};