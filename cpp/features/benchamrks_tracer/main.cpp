
#include "timestamp.hpp"
#include "timestamp_collector.hpp"
#include <unistd.h>
#include <random>
#include <thread>

void f1() {
    TimeStamp();
    int r = (rand() % 5) + 1;
    usleep(r * 1000);
}


void function2() {
    TimeStamp();
    int r = (rand() % 10) + 1;
    usleep(r * 100000);
    printf("r=[%d]\n", r);
}

int main() {
    srand(time(nullptr));
    auto t1 = std::thread([](){
        for (uint32_t i = 0; i < 100; i++) {
            f1();
        }
    });

    auto t2 = std::thread([](){
        for (uint32_t i = 0; i < 100; i++) {
            function2();
        }
    });

    t1.join();
    t2.join();

    TimeStampCollector::get_instance().dump();

    return 0;
}