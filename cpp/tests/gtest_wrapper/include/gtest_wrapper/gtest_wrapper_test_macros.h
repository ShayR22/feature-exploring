#pragma once

#include "gtest_wrapper/gtest_wrapper_reporter.h"
#include "gtest_wrapper/gtest_include_wrapper.h"
#include <initializer_list>

/**
 * @file tlab_tests.h
 * @brief
 * A header containing MACROs for ease of use for gtest tests creation.
 * In gtest in order to create a test we need to create a class, each class
 * represent a test and contains two overridable functions SetUp/TearDown.
 *
 * each test contains:
 *  - test_suite: a name for a classification unit of tests (an example would be Registers)
 *  - test_name: a name for a specific test name (an example would be WriteReg/ReadReg)
 *    Note: the class representing the tests would be a concatenation of the two in order
 *          to have injective functions names and the ability to filter both by test suite/name.
 *
 * - filters: list genrerated by user in order to later filter results
 *
 * - setups: a list of function pointers for setup functions for initializing the state of the system,
 *   before the start of the test, the reason this is a list and not a single function is that some tests
 *   might only require one setup test (example init specific device driver) and some might want to
 *   initialize multiple devices drivers.
 *   Note: setups should be wrapped by SETUPS macro defined below.
 *   Note: tests that doesn't require any setup, can use no_setup list defined in this header
 *
 * - teardowns: a list of function pointers for teardown functions, the setups opposite equivelent.
 *   Note: teardowns should be wrapped by TEARDOWNS macro defined below.
 *   Note: tests that doesn't require any teardown, can use no_teardown list defined in this header
 *
 * tests which contains parameters would contain two additional fields:
 *  - parameters: a tuple defining the values types example would be for test the check temperature threshold
 *    to hold the threshold checked using std::tuple<int8_t> (assuming temperature is bound between -127 to 128)
 *    Note: the tuple should be wrapped by PARAMS macro defined below
 *  - values: a list of comma separated tuple values. the values types should correspond to the parameters defined above,
 *    and a list is used in order to run the same tests with different values, each value set would execute the setups and
 *    teardowns in order to have clean enviornment between tests.
 *    Note: the tuple list should be wrapped by VALUES macro defined below
 *
 */


/**
 * @brief SETUPS and TEARDOWNS are macro to wrap 1..* setup/teardown functions.
 *
 * each macro takes a list of callback function and put them into an initializer
 * list (require C++ version >= 17).
 *
 * the macros takes the initializer list of function pointers and unwrap inside test classes in compile time,
 * thus, functions must be alive in compile time in order to link, this means only floating functions / static class
 * functions can be used.
 * singletons/global objects can't be used as they require a binding from a specific object which would
 * only be initialized in runtime and won't be available in compile time.
 */
#define SETUPS(...) std::initializer_list<void(*)()>{__VA_ARGS__}
#define TEARDOWNS(...) std::initializer_list<void(*)()>{__VA_ARGS__}


/**
 * @brief PARAMETERS_TYPE and VALUES are macros to wrap tuples of values types and actual values to use in the test.
 */
#define PARAMETERS_TYPE(...) __VA_ARGS__
#define VALUES(...) __VA_ARGS__

#define PACK(...) __VA_ARGS__

/**
 * @brief macro to concat test name and filters name into a single name
 */
#define CONCAT_TEST_AND_FILTER_NAME(test, filters) test##filters

/**
 * @brief macro for creating gtest class for test with parameters:
 *  - test_fixture: class test which is the fixture name
 *  - setups: list of setups to embed in the class for overriding test setup
 *  - teardowns: list of teardowns to embed in the class for overriding test teardown
 *  - ...: parameters tuple defining the parameters types.
 *
 * Note: should not be used directly, but only via TLAB_TEST macro
 */
#define _CREATE_PARAMETERIEZED_FIXTURE(test_fixture, setups, teardowns, ...)    \
    class test_fixture : public testing::TestWithParam<__VA_ARGS__> {           \
    protected:                                                                  \
        virtual void SetUp() override {                                         \
            GTestWrapperReporter::get_instance().setup_start();                 \
            for (const auto& setup: setups) {                                   \
                setup();                                                        \
            }                                                                   \
            GTestWrapperReporter::get_instance().setup_end();                   \
        }                                                                       \
                                                                                \
        virtual void TearDown() override {                                      \
            GTestWrapperReporter::get_instance().teardown_start();              \
            for (const auto& teardown: teardowns) {                             \
                teardown();                                                     \
            }                                                                   \
            GTestWrapperReporter::get_instance().teardown_end();                \
        }                                                                       \
    };

/**
 * @brief macro for creating gtest class for test without parameters:
 *  - test_fixture: class test which is the fixture name
 *  - setups: list of setups to embed in the class for overriding test setup
 *  - teardowns: list of teardowns to embed in the class for overriding test teardown
 *
 * Note: should not be used directly, but only via TLAB_TEST_WITHOUT_PARAMS macro
 */
#define _CREATE_NO_PARAMS_FIXTURE(test_fixture, setups, teardowns)                                  \
    class test_fixture : public testing::Test {                                                     \
    protected:                                                                                      \
        virtual void SetUp() override {                                                             \
            GTestWrapperReporter::get_instance().setup_start();                                     \
            for (const auto& setup: setups) {                                                       \
                setup();                                                                            \
            }                                                                                       \
            GTestWrapperReporter::get_instance().setup_end();                                       \
        }                                                                                           \
                                                                                                    \
        virtual void TearDown() override {                                                          \
            GTestWrapperReporter::get_instance().teardown_start();                                  \
            for (const auto& teardown: teardowns) {                                                 \
                teardown();                                                                         \
            }                                                                                       \
            GTestWrapperReporter::get_instance().teardown_end();                                    \
        }                                                                                           \
    };

/**
 * @brief macro to validate tlab test params
 * @param test_suite Checks if the user provide test_suite
 *        How it works - #test_suite will convert the name to string, we need to check if it is bigger than 1 because it
 *                       contains \0
 * @param test_suite Checks if the user provide test_name
 *        How it works - same as test_suite
 * @param setups Checks that the user used the SETUPS macro
 *        How it works - SETUPS macro wraps functions which gets nothing and returns nothing, with initializer_list.
 *                       So we check that the type of setups is initializer_list of those function type
 * @param teardowns Checks that the user used the TEARDOWNS macro
 *        How it works - same as setups
 * @param parameters & values Checks the values types corresponding to the parameters
 *        How it works - we try to create a variable with the parameters and values, if the compiler can create it,
 *                       it means that the values correspond to the parameters
 */
#define _VALIDATE_GTEST_TEST(test_suite, test_name, setups, teardowns, parameters, values...)                                                                           \
    static_assert(sizeof(#test_suite) > 1, "Test suite can't be empty");                                                                                                \
    static_assert(sizeof(#test_name) > 1, "Test name can't be empty");                                                                                                  \
    static_assert(std::is_same<std::remove_const<decltype(setups)>::type, std::initializer_list<void(*)()>>::value, "Please use SETUPS macro on your setups");          \
    static_assert(setups.size() > 0, "SETUPS can't be empty. If you have no setup, use no_setup");                                                                      \
    static_assert(std::is_same<std::remove_const<decltype(teardowns)>::type, std::initializer_list<void(*)()>>::value, "Please use TEARDOWNS macro on your teardowns"); \
    static_assert(teardowns.size() > 0, "TEARDOWNS can't be empty. If you have no teardown, use no_teaddown");                                                          \
    std::initializer_list<parameters> _PARAMS_VALUES_TESTER_##test_suite##test_name{values};

/**
 * @brief macro for creating a test with parameters, view file documentation for full description of
 * the values passed
 */

#define CREATE_GTEST_TEST(test_suite, test_name, filters, setups, teardowns, parameters, values...)                             \
    _VALIDATE_GTEST_TEST(test_suite, test_name, PACK(setups), PACK(teardowns), PARAMETERS_TYPE(parameters), VALUES(values))     \
    _CREATE_PARAMETERIEZED_FIXTURE(test_suite##_##test_name, PACK(setups), PACK(teardowns), PACK(parameters))                   \
    INSTANTIATE_TEST_SUITE_P(test_suite, test_suite##_##test_name, ::testing::Values(values));                                  \
    TEST_P(test_suite##_##test_name, CONCAT_TEST_AND_FILTER_NAME(test_name##_FILTERS_, filters))

/**
 * @brief macro for creating a test with parameters, view file documentation for full desription of
 * the values passed
 */
#define CREATE_GTEST_TEST_WITHOUT_PARAMS(test_suite, test_name, filters, setups, teardowns)                                     \
    _VALIDATE_GTEST_TEST(test_suite, test_name, PACK(setups), PACK(teardowns), PARAMETERS_TYPE(std::tuple<>), VALUES())  \
    _CREATE_NO_PARAMS_FIXTURE(test_suite##_##test_name, PACK(setups), PACK(teardowns))                                          \
    TEST_F(test_suite##_##test_name, CONCAT_TEST_AND_FILTER_NAME(test_name##_FILTERS_, filters))

/**
 * @brief no_setup and no_teardown are both initializer list to use if no setup or teardown is required
 */
[[maybe_unused]] static constexpr auto no_setup = SETUPS([](){});
[[maybe_unused]] static constexpr auto no_teardown = TEARDOWNS([](){});