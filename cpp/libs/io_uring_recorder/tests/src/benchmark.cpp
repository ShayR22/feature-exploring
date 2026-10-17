#include <gtest_wrapper/gtest_wrapper_test_macros.h>
#include <io_uring_recorder/io_uring_recorder.hpp>
#include <io_uring_recorder/rotating_recorder.hpp>
#include "temporary_directory.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {
using Clock = std::chrono::steady_clock;
static constexpr std::uint64_t KiB = 1024ULL;
static constexpr std::uint64_t MiB = 1024ULL * KiB;
static constexpr std::uint64_t GiB = 1024ULL * MiB;

struct Options {
    std::vector<std::size_t> sizes_kib{500, 1024, 2048, 4165};
    std::size_t directories{48};
    std::size_t rotation_size{16};
    std::size_t writes_per_directory{192};
    std::uint32_t queue_depth{64};
    bool unique_buffers{false};
    bool keep_files{false};
};

std::shared_ptr<Sample> make_sample(const std::size_t size_bytes) {
    auto sample = std::make_shared<Sample>();
    sample->data.resize(size_bytes);
    static constexpr std::size_t page_size = 4096;
    for (std::size_t offset = 0; offset < size_bytes; offset += page_size) {
        sample->data[offset] = std::byte{0x5A};
    }
    if (size_bytes > 0) sample->data.back() = std::byte{0xA5};
    return sample;
}

double elapsed_seconds(const Clock::time_point start, const Clock::time_point end) {
    return std::chrono::duration<double>(end - start).count();
}

void print_rate(const std::string& label, const std::uint64_t bytes, const double seconds) {
    const double mib = static_cast<double>(bytes) / static_cast<double>(MiB);
    const double mib_per_second = seconds > 0.0 ? mib / seconds : 0.0;
    std::cout << std::left << std::setw(28) << label << ": " << std::right << std::fixed << std::setprecision(2)
              << seconds << " s, " << mib_per_second << " MiB/s\n";
}
}

CREATE_GTEST_TEST_WITHOUT_PARAMS(IoUringRecorder, Benchmark, WorkloadAndMeasurements, no_setup, no_teardown) {
    Options options;
    const char* unique_buffers = std::getenv("IO_URING_BENCH_UNIQUE_BUFFERS");
    options.unique_buffers = unique_buffers != nullptr && std::string{unique_buffers} == "1";
    const char* keep_files = std::getenv("IO_URING_BENCH_KEEP_FILES");
    options.keep_files = keep_files != nullptr && std::string{keep_files} == "1";

    TemporaryDirectory output_root{"io_uring_bench_data"};
    const auto run_directory = output_root.path() / "run";
    std::filesystem::create_directories(run_directory);

    std::vector<std::shared_ptr<Sample>> reusable_samples;
    reusable_samples.reserve(options.sizes_kib.size());
    for (const std::size_t size_kib : options.sizes_kib) {
        reusable_samples.push_back(make_sample(size_kib * static_cast<std::size_t>(KiB)));
    }
    const std::uint64_t total_writes = options.writes_per_directory * options.directories;
    const std::uint64_t physical_file_slots = options.rotation_size * options.directories;
    std::cout << "io_uring file-write benchmark\n"
              << "Output directory : " << run_directory << '\n'
              << "Queue depth      : " << options.queue_depth << '\n'
              << "Directories      : " << options.directories << '\n'
              << "Rotation size    : " << options.rotation_size << " files/directory\n"
              << "Writes/directory : " << options.writes_per_directory << '\n'
              << "Total writes     : " << total_writes << '\n'
              << "Physical slots   : " << physical_file_slots << '\n'
              << "Unique buffers   : " << (options.unique_buffers ? "yes\n" : "no\n")
              << "Sample sizes     : ";
    for (std::size_t index = 0; index < options.sizes_kib.size(); ++index) {
        if (index != 0) std::cout << ',';
        std::cout << options.sizes_kib[index] << " KiB";
    }
    std::cout << "\n\n";

    {
        IoUringRecorder recorder{RecorderConfig{.root_directory = run_directory, .queue_depth = options.queue_depth}};
        RotatingRecorder rotating_recorder{recorder, RotationConfig{.files_per_directory = options.rotation_size}};
        const auto total_start = Clock::now();
        std::uint64_t bytes_submitted = 0;
        std::uint64_t writes_submitted = 0;
        for (std::uint64_t write_round = 0; write_round < options.writes_per_directory; ++write_round) {
            for (std::size_t directory_index = 0; directory_index < options.directories; ++directory_index) {
                const std::size_t size_index = static_cast<std::size_t>((write_round + directory_index) % options.sizes_kib.size());
                const std::size_t sample_size = options.sizes_kib[size_index] * static_cast<std::size_t>(KiB);
                std::shared_ptr<const Sample> sample = options.unique_buffers
                                                           ? make_sample(sample_size)
                                                           : reusable_samples[size_index];
                rotating_recorder.submit("stream_" + std::to_string(directory_index), std::move(sample));
                bytes_submitted += sample_size;
                ++writes_submitted;
            }
        }
        const auto enqueue_end = Clock::now();
        const FlushStats flush_stats = rotating_recorder.flush();
        const auto total_end = Clock::now();
        const double enqueue_seconds = elapsed_seconds(total_start, enqueue_end);
        const double total_seconds = elapsed_seconds(total_start, total_end);
        const double writes_per_second = total_seconds > 0.0 ? writes_submitted / total_seconds : 0.0;
        const double total_gib = static_cast<double>(bytes_submitted) / static_cast<double>(GiB);
        std::cout << "Results\n-------\n"
                  << "Writes completed            : " << writes_submitted << '\n'
                  << "Bytes written               : " << bytes_submitted << '\n'
                  << "GiB written                 : " << std::fixed << std::setprecision(3) << total_gib << '\n';
        print_rate("Submission phase", bytes_submitted, enqueue_seconds);
        std::cout << std::left << std::setw(28) << "Flush wait for async I/O" << ": " << std::right << std::fixed
                  << std::setprecision(2) << flush_stats.wait_for_async_io_seconds << " s\n";
        std::cout << std::left << std::setw(28) << "Final syncfs" << ": " << std::right << std::fixed
                  << std::setprecision(2) << flush_stats.sync_seconds << " s\n";
        print_rate("Durable end-to-end", bytes_submitted, total_seconds);
        std::cout << std::left << std::setw(28) << "Writes/second" << ": " << std::right << std::fixed
                  << std::setprecision(2) << writes_per_second << "\n\n";
    }
    // The recorder has finished before any of its files are removed.
    if (!options.keep_files) {
        const auto cleanup_start = Clock::now();
        const std::uintmax_t removed = std::filesystem::remove_all(run_directory);
        const auto cleanup_end = Clock::now();
        std::cout << "Cleanup removed " << removed << " filesystem entries in " << std::fixed
                  << std::setprecision(2) << elapsed_seconds(cleanup_start, cleanup_end) << " s\n";
    } else {
        output_root.keep();
        std::cout << "Files kept at: " << run_directory << '\n';
    }
}
