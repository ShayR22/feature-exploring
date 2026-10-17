#include <gtest_wrapper/gtest_wrapper_runner.h>

int main(int argc, char** argv) {
    return GTestWrapperRunner::get_instance().run(argc, argv, *new testing::Environment{});
}
