#include <gtest_wrapper/gtest_wrapper_test_macros.h>
#include <io_uring_recorder/io_uring_recorder.hpp>
#include "temporary_directory.hpp"
#include <io_uring_recorder/rotating_recorder.hpp>

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

namespace {
std::shared_ptr<Sample> make_known_sample(const std::size_t size, const std::uint8_t seed) {
    auto sample = std::make_shared<Sample>();
    sample->data.resize(size);
    for (std::size_t index = 0; index < size; ++index) {
        sample->data[index] = std::byte{static_cast<std::uint8_t>((index * 31U + seed) & 0xFFU)};
    }
    return sample;
}

std::string sample_to_string(const Sample& sample) {
    std::string result(sample.data.size(), '\0');
    for (std::size_t index = 0; index < sample.data.size(); ++index) {
        result[index] = static_cast<char>(std::to_integer<unsigned char>(sample.data[index]));
    }
    return result;
}

std::string read_file(const std::filesystem::path& path) {
    std::ifstream stream{path, std::ios::binary};
    EXPECT_TRUE(stream.is_open()) << "Failed to open " << path;
    return {std::istreambuf_iterator<char>{stream}, std::istreambuf_iterator<char>{}};
}
}

CREATE_GTEST_TEST_WITHOUT_PARAMS(IoUringRecorder, Validation, DirectAndRotation, no_setup, no_teardown) {
    const TemporaryDirectory directory{"io_uring_recorder_validation"};
    const auto& root = directory.path();
    std::filesystem::create_directories(root / "direct");
    {
        IoUringRecorder recorder{RecorderConfig{.root_directory = root, .queue_depth = 1}};
        RotatingRecorder rotating_recorder{recorder, RotationConfig{.files_per_directory = 2}};
        auto direct_sample = make_known_sample(4097, 7);
        const std::string direct_expected = sample_to_string(*direct_sample);
        recorder.submit("direct/known.bin", direct_sample);
        direct_sample.reset();

        auto sample_a = make_known_sample(1024, 20);
        auto sample_b = make_known_sample(1161, 21);
        auto sample_c = make_known_sample(1298, 22);
        auto sample_d = make_known_sample(1435, 23);
        const std::string expected_slot_0 = sample_to_string(*sample_c);
        const std::string expected_slot_1 = sample_to_string(*sample_d);
        rotating_recorder.submit("rotated", std::move(sample_a));
        rotating_recorder.submit("rotated", std::move(sample_b));
        rotating_recorder.submit("rotated", std::move(sample_c));
        rotating_recorder.submit("rotated", std::move(sample_d));
        static_cast<void>(rotating_recorder.flush());

        EXPECT_EQ(read_file(root / "direct/known.bin"), direct_expected);
        EXPECT_EQ(read_file(root / "rotated/sample_0.bin"), expected_slot_0);
        EXPECT_EQ(read_file(root / "rotated/sample_1.bin"), expected_slot_1);
        const auto first_file = std::filesystem::directory_iterator{root / "rotated"};
        EXPECT_EQ(static_cast<std::size_t>(std::distance(first_file, std::filesystem::directory_iterator{})), 2U);
    }
}
