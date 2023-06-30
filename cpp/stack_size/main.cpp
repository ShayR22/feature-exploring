#include <cstdint>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/resource.h>
#include <sstream>

static constexpr uint32_t KB = 1024;
static constexpr uint32_t MB = 1024 * KB;
static constexpr uint32_t GB = 1024 * MB;

struct StackStatus {
    uint64_t current_stack_loc;
    uint64_t start_stack_loc;
    uint32_t remaining_stack;
    uint32_t stack_used;
    uint32_t stack_size;
};

std::string _to_string_with_precision(const std::floating_point auto a_value, const int n = 3) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

std::string _generate_size_string(uint64_t size) {
    const char* units = "Bytes";
    float size_f = size;
    if (size > GB) {
        units = "GB";
        size_f /= GB;
    } else if (size > MB) {
        units = "MB";
        size_f /= MB;
    } else if (size > KB) {
        units = "KB";
        size_f /= KB;
    }
    return _to_string_with_precision(size_f) + "[" + units + "]";
}

StackStatus get_stack_status() {
    char stack_marker{};
    size_t stack_size_tmp{};
    pthread_attr_t attr;
    pthread_getattr_np(pthread_self(), &attr);

    void* stack_start_ptr = nullptr;
    pthread_attr_getstack(&attr, &stack_start_ptr, &stack_size_tmp);

    uint32_t stack_size = static_cast<uint32_t>(stack_size_tmp);
    uint64_t current_stack_loc = reinterpret_cast<uint64_t>(&stack_marker);
    uint64_t start_stack_loc = reinterpret_cast<uint64_t>(stack_start_ptr);
    // logic is not faulty as stack works in reverse meaning the more stack we used the smaller current_stack_loc gets
    // towards the start_stack_loc.
    // example would be when main start remaining_stack == stack_size
    uint32_t remaining_stack = static_cast<uint32_t>(current_stack_loc - start_stack_loc);
    uint32_t stack_used = stack_size - remaining_stack;

    StackStatus status = {
        .current_stack_loc = current_stack_loc,
        .start_stack_loc = start_stack_loc,
        .remaining_stack = remaining_stack,
        .stack_used = stack_used,
        .stack_size = stack_size,
    };

    return status;
}

void print_stack_status() {
    StackStatus status = get_stack_status();
    const auto& [current_stack_loc, start_stack_loc, remaining_stack, stack_used, stack_size] = status;
    std::string remaining_stack_str = _generate_size_string(remaining_stack);
    std::string stack_used_str = _generate_size_string(stack_used);
    std::string stack_size_str = _generate_size_string(stack_size);
    printf("stack_used=%s remaining_stack=%s stack_size=%s current_stack_loc=0x%016lx start_stack_loc=0x%016lx\n",
            stack_used_str.c_str(), remaining_stack_str.c_str(), stack_size_str.c_str(),
            current_stack_loc, start_stack_loc);
    fflush(stdout);
}

void recursive_stack_alloc(uint32_t num_reps, uint32_t bytes_size) {
    if (num_reps == 0) {
        return;
    }

    uint8_t arr[bytes_size];
    printf("arr[0]=%u arr[%u]=%u\n", (uint32_t)arr[0], bytes_size-1, (uint32_t)arr[bytes_size-1]);
    print_stack_status();
    recursive_stack_alloc(--num_reps, bytes_size);
}

// ======================================================================
// ======================================================================
// ======================================================================
// ======================== DEBUGGING CODE START ========================
// ======================================================================
// ======================================================================
// ======================================================================

uint64_t read_stack_size_linux() {
    // Copy paste power
    FILE *file = fopen("/proc/self/maps", "r");
    char line[1024];
    void *result = NULL;
    while (!feof(file)) {
        if (fgets(line, sizeof(line) / sizeof(char), file) == NULL) {
            break;
        }
        unsigned long start, end, offset;
        unsigned int devma, devmi, ino;
        char perms[6];
        char path[128];
        if (sscanf(line, "%lx-%lx %5s %lx %d:%d %d %127s",
                   &start, &end, perms, &offset, &devma, &devmi, &ino, path) != 8) {
            continue; // could not parse. fail gracefully and try again on the next line.
        }
        if (strcmp(path, "[stack]") == 0) { // use [stack:TID] for a thread besides the main thread
            uint64_t stack_size = end - start;
            std::string stack_size_str = _generate_size_string(stack_size);
            printf("Stack found from %lx to %lx (stack_size: %s)\n", start, end, stack_size_str.c_str());
            break;
        }
    }
    fclose(file);
    return 0;
}

uint32_t get_thread_max_stack_size() {
    pthread_attr_t attr;
    size_t stack_size;

    pthread_attr_init(&attr);
    pthread_attr_getstacksize(&attr, &stack_size);
    return static_cast<uint32_t>(stack_size);
}


void stack_alloc_storage_and_use(uint32_t max_stack_size) {
    uint32_t max_stacksize = max_stack_size * 1.1;
    for (uint32_t i = 10; i < max_stacksize; i++) {
        if (i > max_stack_size) {
            printf("i=%u\n", i);
            fflush(stdout);
        }
        uint8_t array[i];
        for (uint32_t j = 0; j < 1; j++) {
            if (j > max_stacksize) {
                printf("j=%u\n", j);
            }
        }
    }
}

void alloc_stack_memory(uint32_t size) {
    uint8_t arr[size];
    if (size > 4 * MB) {
        printf("%u, %u", static_cast<uint32_t>(arr[0]), static_cast<uint32_t>(arr[size-1]));
    }
}

void print_stack_size() {
    uint32_t max_stack_size = get_thread_max_stack_size();
    printf("stack size: %s", _generate_size_string(max_stack_size).c_str());
}

// ====================================================================
// ====================================================================
// ====================================================================
// ======================== DEBUGGING CODE END ========================
// ====================================================================
// ====================================================================
// ====================================================================


int main(int argc, const char* argv[]) {
    // ======================== DEBUGGING CODE START ========================
    // uint32_t stack_size = print_stack_size();
    // // stack_alloc_storage_and_use(stack_size);
    // for (uint32_t i = 1; i < 30; i++) {
    //     uint32_t size = 1 << i;
    //     printf("i=%u\n", i);
    //     fflush(stdout);
    //     alloc_stack_memory(size);
    // }

    // read_stack_size_linux();
    // ======================== DEBUGGING CODE END ========================
    recursive_stack_alloc(8 * KB, KB);
    return 0;
}