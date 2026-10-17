#!/usr/bin/env bash
# Host entry point: forward the pipeline into the development container.
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/scripts/output.sh"
CONTAINER_NAME="${CONTAINER_NAME:-cpp_builder}"
if [[ "${1:-}" == --help || "${1:-}" == -h ]]; then
    exec bash "$SCRIPT_DIR/scripts/docker_build.sh" --help
fi
if ! command -v docker >/dev/null 2>&1; then
    print_error "Docker is required. Run ./cpp/setup/install.sh --help for prerequisites."
    exit 1
fi
if [[ "$(docker inspect -f '{{.State.Running}}' "$CONTAINER_NAME" 2>/dev/null || true)" != true ]]; then
    print_error "Container '$CONTAINER_NAME' is not running. Run ./cpp/setup/install.sh from the repository root."
    exit 1
fi
forward=(-e "HOST_WORKDIR=$SCRIPT_DIR"
    -e "HOST_CONAN_DIR=${CONAN_CACHE_DIR:-$HOME/.cache/feature-exploring/conan2}")
for name in BUILD_TYPE BUILD_GENERATOR TARGET BUILD_TESTS BUILD_EXPERIMENTS \
    USE_UNITY_BUILD INSTALL_PREFIX BUILD_LINKER ENABLE_SANITIZERS ENABLE_THREAD_SANITIZER ENABLE_CLANG_TIDY \
    ENABLE_IWYU ENABLE_CCACHE DISABLE_PCH GTEST_FILTER GTEST_COLOR GTEST_WRAPPER_KEEP_REPORTS \
    CLICOLOR_FORCE NO_COLOR \
    IO_URING_BENCH_KEEP_FILES IO_URING_BENCH_UNIQUE_BUFFERS; do
    if [[ -v "$name" ]]; then forward+=(-e "$name"); fi
done
if [[ -t 1 && ! -v CLICOLOR_FORCE && ! -v NO_COLOR ]]; then
    forward+=(-e CLICOLOR_FORCE=1)
fi
if [[ -t 1 && ! -v GTEST_COLOR && ! -v NO_COLOR ]]; then
    forward+=(-e GTEST_COLOR=yes)
fi
exec docker exec "${forward[@]}" -w /workspace/cpp "$CONTAINER_NAME" \
    bash /workspace/cpp/scripts/docker_build.sh "$@"
