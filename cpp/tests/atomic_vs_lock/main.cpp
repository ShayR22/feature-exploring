


#include <cstdio>
#include <cstdint>
#include <mutex>
#include <atomic>
#include <unistd.h>
#include <thread>

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
        for (uint32_t i = 0; i < 5; i++) {
            usleep(10 * 1000);
            enabled = false;
            usleep(100);
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
        for (uint32_t i = 0; i < 5; i++) {
            usleep(10 * 1000);
            lock.lock();
            usleep(100);
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
    if (argc != 2) {
        printf("%s: a for atomic test, m for mutex test\n", argv[0]);
        return -1;
    }

    char option = argv[1][0];

    if (option == 'a') {
        atomic_test();
    } else if (option == 'm') {
        mutex_test();
    } else {
        printf("%s: a for atomic test, m for mutex test\n", argv[0]);
        return -1;
    }

    return 0;
}