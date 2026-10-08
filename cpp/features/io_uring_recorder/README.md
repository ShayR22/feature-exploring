# io_uring recorder

`io_uring_recorder` is a C++20 static library for asynchronous writes to files beneath a configured root. It owns a
worker and producer queue, retains submitted samples until their writes finish, and reports asynchronous wait and
`syncfs` durations from `flush()`. The ring and request executor are private implementation details. Consumers can use
the direct recorder or the optional rotating filename policy through these headers:

```cpp
#include <io_uring_recorder/io_uring_recorder.hpp>
#include <io_uring_recorder/rotating_recorder.hpp>
```

The direct recorder accepts relative file paths; their parent directories must already exist. `RotatingRecorder`
creates logical directories and writes to `sample_N.bin`, wrapping after the configured number of slots. The imported
rotation implementation retains its existing scheduling behavior, including the possibility that separate queued
writes to the same rotating path overlap.

## Dependencies and build

On Linux, install CMake, a C++20 Clang toolchain, `liburing` development headers/library, `clang-tidy`, and
`include-what-you-use`. The shared wrapper fetches GoogleTest and GoogleMock v1.14.0 at configure time. The build script
enables IWYU and clang-tidy by default and prints the selected tool paths and compiler/IWYU versions. The tools
must parse the project's C++20 sources and standard library; the ordinary build exercises both against the actual
translation units.

From `cpp/`, build the test target in Debug or Release:

```bash
./build.sh --build --parallel-build --build-debug --target io_uring_recorder_test
./build.sh --build --parallel-build --target io_uring_recorder_test
```

Both the feature and test executable are part of the default target set. `build.sh` does not need `sudo` for build
directory operations. Release stripping runs only when the install tree contains binaries or shared libraries; a
normal build with no installed artifacts does not require `llvm-strip`. To install artifacts, use the script's
`--install` option and provide system privileges separately if the chosen install destination requires them.

The recorder library links liburing privately. Configuration reports a clear error when the liburing headers or
library are missing. No CTest registration or GoogleTest discovery is used.

## Tests and reports

The executable has one shared main and exactly two wrapper test cases. Run these commands from `cpp/` so wrapper reports
are created under that directory:

```bash
./build/tests/io_uring_recorder/io_uring_recorder_test \
  --gtest_filter='IoUringRecorder_Validation.*'

./build/tests/io_uring_recorder/io_uring_recorder_test \
  --gtest_filter='IoUringRecorder_Benchmark.*'

./build/tests/io_uring_recorder/io_uring_recorder_test
```

The Validation case checks known direct contents after the caller releases its sample, rotation wraparound, the final
contents of both slots, and the exact file count. It uses queue depth 1 and an exclusively created temporary directory.

The benchmark defaults to the original full workload: 48 logical directories, 16 rotating files per directory, 192
writes per directory, queue depth 64, sample sizes of 500 KiB, 1 MiB, 2 MiB, and 4,165 KiB, and approximately 17 GiB
written. It reports submission time, asynchronous flush wait, final `syncfs` time, durable end-to-end throughput, and
writes per second. By default it reuses one buffer per sample size. Set `IO_URING_BENCH_UNIQUE_BUFFERS=1` to allocate a
fresh sample per write.

Set `IO_URING_BENCH_KEEP_FILES=1` to retain benchmark output after a successful run; its location is printed in the
benchmark output. The default cleans up the output after recording objects finish. Cleanup also runs if a case throws.

Both cases remain enabled without a GoogleTest filter, so an unfiltered invocation runs validation and the full
benchmark.

The wrapper prints captured stdout, including benchmark measurements, to the terminal. Tests leave no report
folders by default. Set `GTEST_WRAPPER_KEEP_REPORTS=1` to also save one report per case to
`report_DD-MM-YYYY_HH-MM-SS/IoUringRecorder_Validation/Validation` or
`report_DD-MM-YYYY_HH-MM-SS/IoUringRecorder_Benchmark/Benchmark`, relative to the process working directory.
Tests exclusively create temporary data directories with `mkdtemp`, rather than deleting predictable paths.
Recording objects are destroyed before automatic cleanup removes those directories.

The wrapper's report retention is opt-in. Its original include shim was empty; the integration fills
that header with GoogleTest and iostream includes needed by the wrapper. The GNU variadic-macro warning exception is
limited to the test executable; the recorder and existing repository targets retain their analysis settings.

The shared clang-tidy configuration is unchanged from the original repository.

`io_uring_setup` must be permitted by the Linux host and its security policy. The test executable can compile
successfully on a host that denies io_uring, but runtime validation and benchmarking require an allowed host.

## Toolchain verified during integration

The integration was built with Clang 18.1.8, clang-tidy 14.0.0, IWYU 0.17 (based on Clang 13.0.1), CMake 3.22.1,
liburing 2.1, and GoogleTest/GoogleMock 1.14.0. Both analysis tools ran on the recorder, wrapper, and test targets in
Debug and Release builds. The complete default Release target set and the full benchmark passed in the integration
environment before the original shared clang-tidy configuration was restored. The restored configuration has not
been rebuilt; those results do not verify the current configuration.
