#!/usr/bin/env bash
# Run this script inside the development container.
# From the host, use cpp/build.sh, which invokes this script through Docker.
# This script builds the C++ project; it does not build the Docker image.
# Build pipeline: Conan 2 -> configure -> build -> optional install / tests.
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
source "$SCRIPT_DIR/scripts/output.sh"
cd "$SCRIPT_DIR"
BUILD_GENERATOR="${BUILD_GENERATOR:-Ninja}"
BUILD_TYPE="${BUILD_TYPE:-Release}"
TARGET="${TARGET:-all}"
BUILD_TESTS="${BUILD_TESTS:-ON}"
BUILD_EXPERIMENTS="${BUILD_EXPERIMENTS:-ON}"
USE_UNITY_BUILD="${USE_UNITY_BUILD:-OFF}"
INSTALL_PREFIX="${INSTALL_PREFIX:-}"
ENABLE_SANITIZERS="${ENABLE_SANITIZERS:-OFF}"
ENABLE_THREAD_SANITIZER="${ENABLE_THREAD_SANITIZER:-OFF}"
BUILD_LINKER="${BUILD_LINKER:-lld}"
ENABLE_CLANG_TIDY="${ENABLE_CLANG_TIDY:-OFF}"
ENABLE_IWYU="${ENABLE_IWYU:-OFF}"
ENABLE_CCACHE="${ENABLE_CCACHE:-OFF}"
DISABLE_PCH="${DISABLE_PCH:-ON}"
DO_CONAN=0 DO_CONFIGURE=0 DO_BUILD=0 DO_INSTALL=0 DO_TEST=0
CLEAN_CMAKE_CACHE=0 CLEAN_BUILD=0 GRAPH_DEPENDENCY=0
CUSTOM_BUILD_DIR=""
JOBS="$(nproc)"
(( JOBS <= 2 )) || JOBS=$((JOBS - 2))
usage() {
    cat <<'EOF'
Usage: ./build.sh [options]

Run from cpp/, or use ./cpp/build.sh from the repository root.
Builds run inside the development container (default: cpp_builder).
Start it with ./cpp/setup/install.sh from the repository root.

Steps
  With no step selected: install dependencies, configure, then build.
  Select one or more steps to run only those steps, in the order below.

  --all                   Install dependencies, configure, then build.
  --conan                 Install dependencies with Conan.
  --configure             Generate the CMake build files.
  --build, -b             Compile the selected target.
  --install, -i           Install existing build artifacts.
  --unittests             Run every executable matching bin/*_test.

Build settings
  --build-type TYPE       Release (default), Debug or RelWithDebInfo.
  --build-debug           Shorthand for --build-type Debug.
  --build-dir DIR         Build directory, relative to cpp/ or absolute.
                          Default: build/<lowercase build type>.
  --target, -t TARGET     CMake target to build (default: all).
  --jobs, -j N            Parallel build jobs (default: automatic).
  --build-generator NAME  CMake generator (default: Ninja).
  --linker NAME           lld (default) or mold.
  --install-prefix DIR    Installation directory.
                          Default: build/<lowercase build type>/install.
                          Relative overrides are beneath the build directory.

Optional tools and sanitizers
  --clang-tidy            Enable clang-tidy during compilation.
  --no-clang-tidy         Disable clang-tidy, including environment settings.
  --iwyu                  Enable Include What You Use during compilation.
  --no-iwyu               Disable IWYU, including environment settings.
  --ccache                Enable the compiler cache.
  --sanitizers            Enable AddressSanitizer and UBSan.
  --thread-sanitizer      Enable ThreadSanitizer instead of ASan/UBSan.
                          Use a separate --build-dir for each sanitizer mode.

Build contents
  --no-tests              Exclude library tests from the configuration.
  --no-experiments        Exclude standalone experiments.
  --no-unity              Disable Unity builds.
  --no-pch                Disable precompiled headers.

Tests and maintenance
  --test-filter EXPR      GoogleTest filter, used with --unittests.
                          No filter is applied by default; benchmarks run too.
  --graph-dependency, -g  Write graph.dot in the build directory.
  --clean-cmake           Delete the build directory before selected steps.
                          Use with --all to regenerate dependencies and CMake.
  --clean-build           Clean compiled artifacts before rebuilding.
                          Requires --build or --all.
  --help, -h              Show this help without requiring Docker.

Examples (from cpp/)
  ./build.sh
  ./build.sh --all --build-debug --target io_uring_recorder_test
  ./build.sh --all --install --unittests
  ./build.sh --unittests --test-filter 'IoUringRecorder_Validation.*'

Use CONTAINER_NAME to select a container other than cpp_builder.
Colors: automatic in terminals; NO_COLOR disables, CLICOLOR_FORCE=1 forces.
EOF
}
need_value() {
    [[ $# -ge 2 && -n "$2" && "$2" != --* ]] || { echo "Missing value for $1" >&2; exit 2; }
}
while [[ $# -gt 0 ]]; do
    case "$1" in
        --help|-h) usage; exit 0 ;;
        --all) DO_CONAN=1; DO_CONFIGURE=1; DO_BUILD=1 ;;
        --conan) DO_CONAN=1 ;;
        --configure) DO_CONFIGURE=1 ;;
        --build|-b) DO_BUILD=1 ;;
        --install|-i) DO_INSTALL=1 ;;
        --unittests) DO_TEST=1 ;;
        --build-type) need_value "$@"; BUILD_TYPE="$2"; shift ;;
        --build-debug) BUILD_TYPE=Debug ;;
        --build-dir) need_value "$@"; CUSTOM_BUILD_DIR="$2"; shift ;;
        --target|-t) need_value "$@"; TARGET="$2"; shift ;;
        --jobs|-j) need_value "$@"; JOBS="$2"; shift ;;
        --parallel-build|-p) ;;
        --no-tests) BUILD_TESTS=OFF ;;
        --no-experiments|--no-tools) BUILD_EXPERIMENTS=OFF ;;
        --no-unity) USE_UNITY_BUILD=OFF ;;
        --no-pch) DISABLE_PCH=ON ;;
        --sanitizers) ENABLE_SANITIZERS=ON ;;
        --thread-sanitizer) ENABLE_THREAD_SANITIZER=ON ;;
        --linker) need_value "$@"; BUILD_LINKER="$2"; shift ;;
        --clang-tidy) ENABLE_CLANG_TIDY=ON ;;
        --iwyu) ENABLE_IWYU=ON ;;
        --ccache) ENABLE_CCACHE=ON ;;
        --no-clang-tidy) ENABLE_CLANG_TIDY=OFF ;;
        --no-iwyu) ENABLE_IWYU=OFF ;;
        --build-generator) need_value "$@"; BUILD_GENERATOR="$2"; shift ;;
        --install-prefix|--build-install-path) need_value "$@"; INSTALL_PREFIX="$2"; shift ;;
        --test-filter) need_value "$@"; export GTEST_FILTER="$2"; shift ;;
        --graph-dependency|-g) GRAPH_DEPENDENCY=1 ;;
        --clean-cmake|--directory-clean|-d) CLEAN_CMAKE_CACHE=1 ;;
        --clean-build|--clean|-c) CLEAN_BUILD=1 ;;
        *) echo "Unknown option: $1" >&2; usage >&2; exit 2 ;;
    esac
    shift
done
if [[ ! -f /.dockerenv ]]; then
    echo "Run this script inside the development container; from the host use cpp/build.sh." >&2
    exit 1
fi
case "$BUILD_TYPE" in
    Release) profile=default; type_dir=release ;;
    Debug) profile=debug; type_dir=debug ;;
    RelWithDebInfo) profile=relwithdebinfo; type_dir=relwithdebinfo ;;
    *) echo "Invalid build type: $BUILD_TYPE" >&2; exit 2 ;;
esac
[[ "$JOBS" =~ ^[1-9][0-9]*$ ]] || { echo "Invalid job count: $JOBS" >&2; exit 2; }
BUILD_DIR="${CUSTOM_BUILD_DIR:-build/$type_dir}"
if (( CLEAN_BUILD && ! DO_BUILD )); then echo "--clean-build requires --build" >&2; exit 2; fi
if (( ! DO_CONAN && ! DO_CONFIGURE && ! DO_BUILD && ! DO_INSTALL && ! DO_TEST )); then
    DO_CONAN=1 DO_CONFIGURE=1 DO_BUILD=1
fi
if (( CLEAN_CMAKE_CACHE )); then
    # Restrict cleanup to a CMake build directory, never the source or Conan cache.
    resolved="$(realpath -m "$BUILD_DIR")"
    case "$resolved" in "$SCRIPT_DIR/build/"*) rm -rf -- "$resolved" ;;
        *) echo "--clean-cmake requires a directory beneath $SCRIPT_DIR/build" >&2; exit 2 ;;
    esac
fi
mkdir -p "$BUILD_DIR"
BUILD_DIR="$(realpath "$BUILD_DIR")"
INSTALL_PREFIX="${INSTALL_PREFIX:-$SCRIPT_DIR/build/$type_dir/install}"
case "$INSTALL_PREFIX" in
    /*) ;;
    *) INSTALL_PREFIX="$BUILD_DIR/$INSTALL_PREFIX" ;;
esac
if [[ "$ENABLE_SANITIZERS" == ON && "$ENABLE_THREAD_SANITIZER" == ON ]]; then
    echo "--sanitizers and --thread-sanitizer require separate builds" >&2
    exit 2
fi
if (( DO_CONAN )); then
    print_step "Conan install" "Installing Conan dependencies ($BUILD_TYPE)"
    conan install conan/conanfile.py --output-folder "$BUILD_DIR/conan" \
        --profile:host "conan/$profile" --profile:build conan/default \
        --settings:host build_type="$BUILD_TYPE" \
        --options "&:build_tests=$([[ $BUILD_TESTS == ON ]] && echo True || echo False)" \
        --options "&:build_experiments=$([[ $BUILD_EXPERIMENTS == ON ]] && echo True || echo False)" \
        --lockfile conan/conan.lock --build=missing
fi
if (( DO_CONFIGURE )); then
    print_step "CMake configure" "Configuring $BUILD_TYPE in $BUILD_DIR"
    cmake -G "$BUILD_GENERATOR" -S . -B "$BUILD_DIR" \
        -DCMAKE_TOOLCHAIN_FILE="$BUILD_DIR/conan/conan_toolchain.cmake" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX" \
        -DBUILD_TESTS="$BUILD_TESTS" -DBUILD_EXPERIMENTS="$BUILD_EXPERIMENTS" \
        -DUSE_UNITY_BUILD="$USE_UNITY_BUILD" -DDISABLE_PCH="$DISABLE_PCH" \
        -DENABLE_SANITIZERS="$ENABLE_SANITIZERS" -DENABLE_THREAD_SANITIZER="$ENABLE_THREAD_SANITIZER" \
        -DBUILD_LINKER="$BUILD_LINKER" -DENABLE_CLANG_TIDY="$ENABLE_CLANG_TIDY" \
        -DENABLE_IWYU="$ENABLE_IWYU" -DENABLE_CCACHE="$ENABLE_CCACHE"
    # Export a host-readable compilation database for editor tooling.
    python3 - "$BUILD_DIR/compile_commands.json" <<'PYTHON'
import json, os, pathlib, sys
source = pathlib.Path(sys.argv[1])
if source.exists():
    entries = json.loads(source.read_text())
    substitutions = [(os.getcwd(), os.environ.get("HOST_WORKDIR", os.getcwd()))]
    if os.environ.get("HOST_CONAN_DIR"):
        substitutions.append((os.environ.get("CONAN_HOME", "/home/builder/.conan2"),
                              os.environ["HOST_CONAN_DIR"]))
    def translate(value):
        if isinstance(value, str):
            for original, replacement in substitutions:
                value = value.replace(original, replacement)
        elif isinstance(value, list):
            value = [translate(item) for item in value]
        elif isinstance(value, dict):
            value = {key: translate(item) for key, item in value.items()}
        return value
    pathlib.Path("compile_commands.json").write_text(json.dumps(translate(entries), indent=2) + "\n")
PYTHON
fi
if (( DO_BUILD )); then
    print_step "CMake build" "Building $TARGET ($BUILD_TYPE)"
    cmd=(cmake --build "$BUILD_DIR" --target "$TARGET" --parallel "$JOBS")
    (( ! CLEAN_BUILD )) || cmd+=(--clean-first)
    "${cmd[@]}"
fi
if (( DO_INSTALL )); then
    print_step "CMake install" "Installing artifacts to $INSTALL_PREFIX"
    cmake --install "$BUILD_DIR" --prefix "$INSTALL_PREFIX"
fi
if (( GRAPH_DEPENDENCY )); then
    print_step "Dependency graph"
    cmake --graphviz="$BUILD_DIR/graph.dot" "$BUILD_DIR"
fi
if (( DO_TEST )); then
    print_step "Unit tests"
    test_count=0
    test_status=0
    for test_binary in "$BUILD_DIR"/bin/*_test; do
        [[ -f "$test_binary" && -x "$test_binary" ]] || continue
        (( ++test_count ))
        print_message '1;34' "Running ${test_binary##*/}."
        if ! "$test_binary"; then
            print_error "FAILED: ${test_binary##*/}"
            test_status=1
        fi
    done
    if (( test_count == 0 )); then
        print_error "No executable *_test binaries found in $BUILD_DIR/bin. Build tests first."
        exit 1
    fi
    if (( test_status == 0 )); then
        print_success "All $test_count test binaries passed."
    else
        print_error "One or more test binaries failed."
    fi
    exit "$test_status"
fi
print_success "Done."
