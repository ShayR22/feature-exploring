#include <functional>
#include <vector>
#include <cstdio>
#include <cstdint>

struct Task {
    std::function<void()> cb;
};

static std::vector<Task> tasks{};

static void add_task() {
    std::function<void()> print_i_j = []() {
        uint32_t i = 1;
        uint32_t j = 2;
        printf("hello i=%u, j=%u\n", i, j);
    };

    tasks.push_back({print_i_j});
}

static void add_task(int val) {
    uint32_t i = val % 3;
    uint32_t j = val % 5;
    std::function<void()> print_i_j = [i, j]() {
        printf("hello i=%u, j=%u\n", i, j);
    };

    tasks.push_back({print_i_j});
}

static void execute_tasks() {
    for (const auto& task: tasks) {
        task.cb();
    }

    tasks.clear();
}

int main() {
    for (int i = 0; i < 5; i++) {
        add_task(i);
    }

    execute_tasks();

    return 0;
}