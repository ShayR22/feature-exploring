FROM ubuntu:22.04 AS toolchain

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates curl gnupg git python3 python3-pip python3-venv \
    build-essential cmake ninja-build ccache pkg-config liburing-dev \
    gdb gdbserver valgrind ripgrep xz-utils \
    libboost-filesystem1.74.0 libboost-iostreams1.74.0 libboost-system1.74.0 \
    libboost-container1.74.0 libboost-program-options1.74.0 \
    libdw1 libelf1 libunwind8 libpcre2-8-0 libzstd1 libcap2 \
    libprotobuf23 libre2-9 libcapstone4 libabsl20210324 \
    && curl -fsSL https://apt.llvm.org/llvm-snapshot.gpg.key | gpg --dearmor -o /usr/share/keyrings/llvm.gpg \
    && echo "deb [signed-by=/usr/share/keyrings/llvm.gpg] https://apt.llvm.org/jammy/ llvm-toolchain-jammy-18 main" > /etc/apt/sources.list.d/llvm.list \
    && echo "deb [signed-by=/usr/share/keyrings/llvm.gpg] https://apt.llvm.org/jammy/ llvm-toolchain-jammy-20 main" >> /etc/apt/sources.list.d/llvm.list \
    && apt-get update && apt-get install -y --no-install-recommends \
    clang-18 llvm-18 lld-18 libclang-rt-18-dev clangd-18 \
    clang-20 clang-tools-20 clang-tidy-20 clang-format-20 \
    llvm-20 llvm-20-dev libclang-20-dev libclang-cpp20-dev \
    && rm -rf /var/lib/apt/lists/*

# Application builds use Clang 18; standalone analysis/refactoring tools use LLVM 20.
ENV CC=clang-18 CXX=clang++-18

FROM toolchain AS tools-builder
ARG TOOL_BUILD_JOBS=4
ENV TOOL_BUILD_JOBS=${TOOL_BUILD_JOBS}
RUN apt-get update && apt-get install -y --no-install-recommends \
    libboost-filesystem-dev libboost-iostreams-dev libboost-system-dev \
    libboost-container-dev libboost-program-options-dev \
    libdw-dev libelf-dev libunwind-dev libpcre2-dev libzstd-dev zlib1g-dev \
    libprotobuf-dev protobuf-compiler libre2-dev libcapstone-dev libabsl-dev \
    libcap-dev flex bison \
    && rm -rf /var/lib/apt/lists/*
COPY install_tools.sh /tmp/install_tools.sh
RUN --mount=type=cache,target=/var/cache/tool-sources bash /tmp/install_tools.sh cppcheck
RUN --mount=type=cache,target=/var/cache/tool-sources bash /tmp/install_tools.sh iwyu
RUN --mount=type=cache,target=/var/cache/tool-sources bash /tmp/install_tools.sh heaptrack
RUN --mount=type=cache,target=/var/cache/tool-sources bash /tmp/install_tools.sh bloaty
RUN --mount=type=cache,target=/var/cache/tool-sources bash /tmp/install_tools.sh mold
# DWZ needs xxHash headers; perf discovers its event-table Python via python3-config.
RUN apt-get update && apt-get install -y --no-install-recommends libxxhash-dev python3-dev \
    && rm -rf /var/lib/apt/lists/*
RUN --mount=type=cache,target=/var/cache/tool-sources bash /tmp/install_tools.sh dwz
RUN --mount=type=cache,target=/var/cache/tool-sources \
    ln -s /usr/bin/clang-18 /usr/local/bin/clang \
    && ln -s /usr/bin/clang++-18 /usr/local/bin/clang++ \
    && ln -s /usr/bin/python3 /usr/local/bin/python \
    && PYTHON=/usr/bin/python3 JOBS="$TOOL_BUILD_JOBS" bash /tmp/install_tools.sh perf

FROM toolchain AS development
# Match host ownership for files written into bind-mounted directories.
ARG DEV_UID=1000
ARG DEV_GID=1000
COPY --from=tools-builder /opt/development-tools/usr/local/ /usr/local/

RUN apt-get update && apt-get install -y --no-install-recommends libxxhash0 \
    && rm -rf /var/lib/apt/lists/* \
    && python3 -m venv /opt/conan \
    && /opt/conan/bin/pip install --no-cache-dir conan==2.33.0 \
    && groupadd --gid ${DEV_GID} builder \
    && useradd --uid ${DEV_UID} --gid ${DEV_GID} --create-home --shell /bin/bash builder \
    && ln -s /usr/bin/clang-18 /usr/local/bin/clang \
    && ln -s /usr/bin/clang++-18 /usr/local/bin/clang++ \
    && ln -s /usr/bin/clang-tidy-20 /usr/local/bin/clang-tidy \
    && ln -s /usr/bin/clang-format-20 /usr/local/bin/clang-format \
    && ln -s /usr/bin/clang-query-20 /usr/local/bin/clang-query \
    && ln -s /usr/bin/llvm-dwarfdump-20 /usr/local/bin/llvm-dwarfdump \
    && ldconfig

ENV PATH=/opt/conan/bin:$PATH
ENV CC=clang-18 CXX=clang++-18
# Conan defaults to $HOME/.conan2, matching its persistent Compose mount.
ENV CCACHE_DIR=/home/builder/.ccache
USER builder
WORKDIR /workspace/cpp
CMD ["bash"]
