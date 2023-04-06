#include "cli11/CLI11.hpp"
#include <random>
#include <iostream>
#include <vector>


static uint8_t rand_in_range(uint8_t start, uint8_t stop) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(start, stop); // define the range

    return distr(gen);
}

class PwmController {
public:
    int bcast_set_duty_cycle(const uint8_t duty_cycle) {
        if (duty_cycle > 100) {
            printf("Invalid duty_cycle=%u, must be 0-100\n", duty_cycle);
            return -1;
        }
        _duty_cycle = duty_cycle;
        return 0;
    }

    int bcast_get_duty_cycle(std::vector<uint8_t>& duty_cycles) {
        duty_cycles.clear();
        duty_cycles.resize(_num_fans);
        for (uint8_t i = 0; i < _num_fans; i++) {
            duty_cycles[i] = _duty_cycle;
        }

        return 0;
    }

    int bcast_get_faults(std::vector<bool>& faults) {
        faults.clear();
        faults.resize(_num_fans);
        for (uint8_t i = 0; i < _num_fans; i++) {
            faults[i] = false;
        }
        return 0;
    }
private:
    uint8_t _num_fans{rand_in_range(1, 3)};
    uint8_t _duty_cycle{0};
};


int main(int argc, char** argv) {
    CLI::App app{"PWM Controller CLI"};

    // initialize PwmController object
    PwmController pwm_controller;

    // add subcommand for setting duty cycle
    auto set_duty_cycle = app.add_subcommand("set_duty_cycle", "Set the duty cycle for all channels");
    uint8_t duty_cycle = 0;
    set_duty_cycle->add_option("-d,--duty_cycle", duty_cycle, "Duty cycle value (0-255)")->required();
    set_duty_cycle->callback([&]() {
        int result = pwm_controller.bcast_set_duty_cycle(duty_cycle);
        if (result == 0) {
            std::cout << "Duty cycle set successfully!" << std::endl;
        } else {
            std::cerr << "Error setting duty cycle. Code: " << result << std::endl;
        }
    });

    // add subcommand for getting duty cycles
    auto get_duty_cycles = app.add_subcommand("get_duty_cycles", "Get the duty cycles for all channels");
    get_duty_cycles->callback([&]() {
        std::vector<uint8_t> duty_cycles;
        int result = pwm_controller.bcast_get_duty_cycle(duty_cycles);
        if (result == 0) {
            std::cout << "Duty cycles:";
            for (auto dc : duty_cycles) {
                std::cout << " " << (int)dc;
            }
            std::cout << std::endl;
        } else {
            std::cerr << "Error getting duty cycles. Code: " << result << std::endl;
        }
    });

    // add subcommand for getting faults
    auto get_faults = app.add_subcommand("get_faults", "Get the fault status for all channels");
    get_faults->callback([&]() {
        std::vector<bool> faults;
        int result = pwm_controller.bcast_get_faults(faults);
        if (result == 0) {
            std::cout << "Fault status:";
            for (auto f : faults) {
                std::cout << " " << (f ? "true" : "false");
            }
            std::cout << std::endl;
        } else {
            std::cerr << "Error getting fault status. Code: " << result << std::endl;
        }
    });

    // run the CLI
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return app.exit(e);
    }

    return 0;
}