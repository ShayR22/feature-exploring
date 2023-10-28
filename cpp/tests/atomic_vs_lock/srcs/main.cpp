#include <cstdio>
#include <cstdint>
#include <mutex>
#include <atomic>
#include <unistd.h>
#include <thread>
#include <span>

static constexpr uint8_t SHORT_SLEEP_US = 100;
static constexpr uint32_t LONG_SLEEP_US = 10 * 1000;
static constexpr uint8_t NUM_TOGGLES = 5;

void atomic_test() {
    std::atomic<bool> enabled = true;
    uint64_t counter = 0;

    auto counter_lambda = [&]() {
        while (true) {
            if (enabled) {
                counter++;
            }
        }
    };

    auto counter_stopper_lambda =  [&]() {

        for (uint32_t i = 0; i < NUM_TOGGLES; i++) {
            usleep(LONG_SLEEP_US);
            enabled = false;
            usleep(SHORT_SLEEP_US);
            enabled = true;
        }
        enabled = false;
    };

    std::thread t1(counter_stopper_lambda);
    std::thread t2(counter_lambda);
    t1.join();

    printf("counter=[%lu]\n", counter);
    exit(0);
}


void mutex_test() {
    std::mutex lock{};
    uint64_t counter = 0;

    auto counter_lambda = [&]() {
        while (true) {
            lock.lock();
            counter++;
            lock.unlock();
        }
    };

    auto counter_stopper_lambda =  [&]() {
        for (uint32_t i = 0; i < NUM_TOGGLES; i++) {
            usleep(LONG_SLEEP_US);
            lock.lock();
            usleep(SHORT_SLEEP_US);
            lock.unlock();
        }
        lock.lock();
    };


    std::thread t1(counter_stopper_lambda);
    std::thread t2(counter_lambda);
    t1.join();

    printf("counter=[%lu]\n", counter);
    exit(0);
}

int main(int argc, const char* argv[]) {
    auto args = std::span(argv, size_t(argc));
    const char* program_name = args[0];
    if (argc != 2) {
        printf("%s: a for atomic test, m for mutex test\n", program_name);
        return -1;
    }

    char option = args[1][0];

    if (option == 'a') {
        atomic_test();
    } else if (option == 'm') {
        mutex_test();
    } else {
        printf("%s: a for atomic test, m for mutex test\n", program_name);
        return -1;
    }

    return 0;
}