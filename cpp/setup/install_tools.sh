#!/usr/bin/env bash
# Build pinned tools in the intermediate image; install only their artifacts.
set -euo pipefail

JOBS="${TOOL_BUILD_JOBS:-4}"
INSTALL_ROOT=/opt/development-tools
SOURCE_ROOT=/tmp/development-tools
mkdir -p "$SOURCE_ROOT" "$INSTALL_ROOT/usr/local"

fetch_source() {
    local name="$1" url="$2" digest="$3"
    local archive="/var/cache/tool-sources/$name.tar"
    mkdir -p /var/cache/tool-sources "$SOURCE_ROOT/$name"
    if [[ ! -f "$archive" ]]; then
        curl --fail --location --retry 3 "$url" -o "$archive"
    fi
    printf '%s  %s\n' "$digest" "$archive" | sha256sum --check
    tar -xf "$archive" --strip-components=1 -C "$SOURCE_ROOT/$name"
}

build_cmake_tool() {
    local name="$1"
    shift
    cmake -S "$SOURCE_ROOT/$name" -B "$SOURCE_ROOT/$name-build" -G Ninja \
        -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local "$@"
    cmake --build "$SOURCE_ROOT/$name-build" --parallel "$JOBS"
    DESTDIR="$INSTALL_ROOT" cmake --install "$SOURCE_ROOT/$name-build"
}

case "${1:-}" in
    cppcheck)
        fetch_source cppcheck \
            https://codeload.github.com/cppcheck-opensource/cppcheck/tar.gz/refs/tags/2.22.0 \
            d74945deb2d50393430e07596b766f8a779512c7f60dac2a30ea64e059ece57b
        build_cmake_tool cppcheck -DBUILD_GUI=OFF -DBUILD_TESTING=OFF
        ;;
    iwyu)
        fetch_source iwyu \
            https://include-what-you-use.org/downloads/include-what-you-use-0.24.src.tar.gz \
            a23421ceff601d3ea215e8fa9292bfa8ca39eb1ac2098dbbedfc6cfe65541c10
        build_cmake_tool iwyu -DCMAKE_PREFIX_PATH=/usr/lib/llvm-20 \
            -DIWYU_LINK_CLANG_DYLIB=ON -DIWYU_RESOURCE_RELATIVE_TO=clang \
            -DIWYU_RESOURCE_DIR=../lib/clang/20
        ;;
    heaptrack)
        fetch_source heaptrack \
            https://download.kde.org/Attic/heaptrack/1.5.0/heaptrack-1.5.0.tar.xz \
            a278d9d8f91e8bfb8a1c2f5b73eecab47fd45d0693f5dbea637536413cec2ea5
        build_cmake_tool heaptrack -DHEAPTRACK_BUILD_GUI=OFF -DBUILD_TESTING=OFF
        ;;
    bloaty)
        # Pin a revision with DWARF 5 support. Build/install only; never run Bloaty.
        git clone --filter=blob:none --no-checkout https://github.com/google/bloaty.git \
            "$SOURCE_ROOT/bloaty"
        git -C "$SOURCE_ROOT/bloaty" checkout f1a83e63f36beb628dcf022e1201a2eb391a22a6
        build_cmake_tool bloaty -DBUILD_TESTING=OFF -DBLOATY_ENABLE_CMAKETARGETS=OFF \
            -DBLOATY_PREFER_SYSTEM_ABSL=OFF
        ;;
    mold)
        case "$(uname -m)" in
            x86_64) digest=6ff270c9bf07d2bec5c98aa324eb7c4daf6a1a4d815c05ff1708049616047855 ;;
            *) echo 'The pinned Mold distribution currently supports x86_64.' >&2; exit 1 ;;
        esac
        fetch_source mold \
            https://github.com/rui314/mold/releases/download/v2.42.1/mold-2.42.1-x86_64-linux.tar.gz \
            "$digest"
        cp -a "$SOURCE_ROOT/mold/." "$INSTALL_ROOT/usr/local/"
        ;;
    dwz)
        fetch_source dwz https://sourceware.org/pub/dwz/releases/dwz-0.17.tar.gz \
            84954c5a68bfc21178c7e050c7f0429ab27cd302cee2668f491cb8b94ab65d55
        make -C "$SOURCE_ROOT/dwz" -j "$JOBS" CC=clang-18
        install -Dm755 "$SOURCE_ROOT/dwz/dwz" "$INSTALL_ROOT/usr/local/bin/dwz"
        ;;
    perf)
        # Microsoft WSL2 kernel 6.18.40.1, matching the current development host.
        fetch_source perf \
            https://codeload.github.com/microsoft/WSL2-Linux-Kernel/tar.gz/14794180686c2fb6307fbe359c359bec765249f3 \
            d472baf81cd0514d30f1b29904c5cb57a77ce09bba1c22ab523648cb1f0ac451
        make -C "$SOURCE_ROOT/perf/tools/perf" -j "$JOBS" \
            CC=clang-18 CXX=clang++-18 WERROR=0 \
            NO_LIBPYTHON=1 NO_LIBBPF=1 NO_LIBTRACEEVENT=1 \
            NO_SLANG=1 NO_LIBNUMA=1 NO_LIBBABELTRACE=1 NO_LIBDEBUGINFOD=1
        install -Dm755 "$SOURCE_ROOT/perf/tools/perf/perf" "$INSTALL_ROOT/usr/local/bin/perf"
        ;;
    *) echo "Usage: $0 cppcheck|iwyu|heaptrack|bloaty|mold|dwz|perf" >&2; exit 2 ;;
esac
rm -rf "$SOURCE_ROOT"
