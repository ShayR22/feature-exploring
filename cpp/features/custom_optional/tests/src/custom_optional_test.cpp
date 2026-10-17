#include <custom_optional/custom_optional.hpp>
#include <gtest_wrapper/gtest_wrapper_runner.h>
#include <gtest_wrapper/gtest_wrapper_test_macros.h>

#include <array>

CREATE_GTEST_TEST_WITHOUT_PARAMS(CustomOptional, SmallAllocation, StoresValue, no_setup, no_teardown) {
    const CustomOptional<int> optional{42};

    ASSERT_TRUE(optional.has_value());
    EXPECT_EQ(optional.value(), 42);
}

CREATE_GTEST_TEST_WITHOUT_PARAMS(CustomOptional, LargeAllocation, StoresValue, no_setup, no_teardown) {
    const CustomOptional<std::array<int, 8>> optional{std::array<int, 8>{42}};

    ASSERT_TRUE(optional.has_value());
    EXPECT_EQ(optional.value().front(), 42);
    EXPECT_EQ(sizeof(optional), sizeof(void*));
}

int main(int argc, char** argv) {
    return GTestWrapperRunner::get_instance().run(argc, argv, *new testing::Environment{});
}
