#!/bin/bash -e

SEPARATION_LINE="================================================================================"

# currently const and can't be changed
BUILD_PATH="build"

# Build Flags
IS_BUILD=0
IS_BUILD_PARALLEL=0
IS_CLEAN=0
IS_BUILD_VERBOSE=0
IS_DIR_CLEAN=0
CMAKE_BUILD_TYPE="Release"
IS_CLANG_BUILD_ANALYZER=OFF
IS_IWYU=ON
IS_CLANG_TIDY=ON
# Holds all target to build
TARGET=()


# Install Flags
IS_INSTALL=0
BUILD_INSTALL_PATH="$(pwd)/${BUILD_PATH}/install"
BUILD_INSTALL_BIN_PATH="${BUILD_INSTALL_PATH}/bin"
BUILD_INSTALL_LIB_PATH="${BUILD_INSTALL_PATH}/lib"

# Graph Dependency Flag
IS_GRAPH_DEPENDENCY=0

function usage() {
    echo -e "\nBuild Flags"
    echo "[-b | --build]            build the project"
    echo "[-p | --parallel-build]   add parallel flag to use all cores"
    echo "[-c | --clean]            clean the targets and dependencies"
    echo "[-v | --verbose]          set build to verbose"
    echo "[-d | --directory-clean]  clean all caches by rm -rf build directory"
    echo "[--build-debug]           set CMAKE_BUILD_TYPE to Debug default=${CMAKE_BUILD_TYPE}"
    echo "[--clang-build-analyzer]  add build metrics for clang build analyzer default=${IS_CLANG_BUILD_ANALYZER}"
    echo "[--no-iwyu]                  add usage of include-what-you-use tool should be only used with clang++ default=${IS_IWYU}"
    echo "[--no-clang-tidy]            add usage of clang-tidy tool should be only used with clang++ default=${IS_CLANG_TIDY}"
    echo "[-t | --target]           build specific target"

    echo -e "\nInstall Flags"
    echo "[-i | --install]          install output into installer tree to be packaged / deployed"
    echo "[--build-install-path]    set BUILD_INSTALL_PATH where targets would be installed. default=${BUILD_INSTALL_PATH}"

    echo -e "\nGraph Dependency Flag"
    echo "[-g | --graph-dependency] create a dot file for viewing depencdency of each target / entire project"

    echo -e "\n[-h | --help]        display this help mesasage\n"
}

# Process arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -b | --build)
            IS_BUILD=1
            ;;
        -p | --parallel-build)
            IS_BUILD_PARALLEL=1
            ;;
        -c | --clean)
            IS_CLEAN=1
            ;;
        -d | --directory-clean)
            IS_DIR_CLEAN=1
            ;;
        -v | --verbose-build)
            IS_BUILD_VERBOSE=1
            ;;
        --build-debug)
            CMAKE_BUILD_TYPE="Debug"
            ;;
        --clang-build-analyzer)
            IS_CLANG_BUILD_ANALYZER=ON
            ;;
        --no-iwyu)
            IS_IWYU=OFF
            ;;
        --no-clang-tidy)
            IS_CLANG_TIDY=OFF
            ;;
        -t | --target)
            shift
            TARGET+=("$1")
            ;;

        -i | --install)
            IS_INSTALL=1
            ;;
        --build-install-path)
            shift
            BUILD_INSTALL_PATH="$1"
            # update bin/lib install paths
            BUILD_INSTALL_BIN_PATH="${BUILD_INSTALL_PATH}/bin"
            BUILD_INSTALL_LIB_PATH="${BUILD_INSTALL_PATH}/lib"
            ;;

        -g | --graph-dependency)
            IS_GRAPH_DEPENDENCY=1
            ;;

        -h | --help)
            usage
            exit 0
            ;;
        *)
            echo "Invalid argument: $1"
            usage
            exit 1
            ;;
    esac
    shift
done

# create build dir if doesn't exist
if [[ ! -d $BUILD_PATH ]]; then
    mkdir $BUILD_PATH
fi

if [[ $IS_DIR_CLEAN -ne 0 ]]; then
    sudo rm -rf $BUILD_PATH
    mkdir $BUILD_PATH
fi

pushd $BUILD_PATH

if [[ ! -d $BUILD_INSTALL_BIN_PATH ]]; then
    mkdir -p $BUILD_INSTALL_BIN_PATH
fi

if [[ ! -d $BUILD_INSTALL_LIB_PATH ]]; then
    mkdir -p $BUILD_INSTALL_LIB_PATH
fi


c_compiler_used=$(which clang)
cxx_compiler_used=$(which clang++)

# c_compiler_used=$(which gcc)
# cxx_compiler_used=$(which g++)

echo "c_compiler_used=$c_compiler_used cxx_compiler_used=$cxx_compiler_used"

export CC="${c_compiler_used}"
export CXX="${cxx_compiler_used}"

# base cmake command
cmake_cmd="cmake ../"

# add compiler used
cmake_cmd="$cmake_cmd -DCMAKE_C_COMPILER=${CC} -DCMAKE_CXX_COMPILER=${CXX}"
# add build type (release/debug)
cmake_cmd="$cmake_cmd -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
# add install path
cmake_cmd="$cmake_cmd -DBUILD_INSTALL_PATH=${BUILD_INSTALL_PATH}"

if [[ $IS_BUILD_VERBOSE -ne 0 ]]; then
    cmake_cmd="$cmake_cmd -DCMAKE_MESSAGE_LOG_LEVEL=VERBOSE"
fi

# add tools used
cmake_cmd="$cmake_cmd -DUSE_IWYU=${IS_IWYU} -DUSE_CLANG_TIDY=${IS_CLANG_TIDY}"
cmake_cmd="$cmake_cmd -DUSE_CLANG_BUILDER_ANALYZER=${IS_CLANG_BUILD_ANALYZER}"

# add prefix install path
cmake_cmd="$cmake_cmd -DCMAKE_INSTALL_PREFIX=$(pwd)"

echo ${SEPARATION_LINE}
echo "Generating CMake cmake_cmd=$cmake_cmd"
$cmake_cmd


if [[ ${IS_BUILD} -ne 0 ]]; then
    echo ${SEPARATION_LINE}
    build_cmd="cmake --build ."

    if [[ ${IS_BUILD_PARALLEL} -ne 0 ]]; then
        build_cmd="$build_cmd --parallel"
    fi

    if [[ ${IS_CLEAN} -ne 0 ]]; then
        build_cmd="$build_cmd --clean-first"
    fi

    if [[ ${#TARGET[@]} -ne 0 ]]; then
        cmake_cmd_targets="--target"
        for target in "${TARGET[@]}"; do
            cmake_cmd_targets="$cmake_cmd_targets $target"
        done
        build_cmd="$build_cmd $cmake_cmd_targets"
    fi

    if [[ $IS_BUILD_VERBOSE -ne 0 ]]; then
        build_cmd="$build_cmd --verbose"
    fi

    echo "Building Project with cmd=$build_cmd"
    sudo $build_cmd
fi

if [[ ${IS_INSTALL} -ne 0 ]]; then
    echo ${SEPARATION_LINE}
    echo "Intalling Project"
    sudo cmake --install .
fi

if [[ ${IS_GRAPH_DEPENDENCY} -ne 0 ]]; then
    echo ${SEPARATION_LINE}
    echo "Generating dependecy graph"
    sudo cmake --graphviz=./graph/graph .
fi

# Symbol strip binaries and shared library
if [[ "${CMAKE_BUILD_TYPE}" == "Release" ]]; then
    echo ${SEPARATION_LINE}
    # when using llvm/clang strip is done by llvm-strip tool which comes as
    # part of the llvm toolset. following lines would match the clang version
    # to llvm-version meaning if we use clang 11.0.1 we would like to use llvm-strip-11
    clang_version_output=$(clang --version)
    llvm_major_version=$(echo "$clang_version_output" | grep -oP '(?<=version )\d+' | awk -F. '{print $1}')
    llvm_strip_name="llvm-strip-${llvm_major_version}"
    echo "llvm_strip_name=${llvm_strip_name}"

    llvm_exec_path="$(which $llvm_strip_name)"
    echo "llvm_exec_path=${llvm_exec_path}"

    echo "Going to strip binaries"
    for f in `find "${BUILD_INSTALL_PATH}/bin" -type f`; do
        echo "stripping $f"
        sudo $llvm_exec_path -s $f
    done

    echo "Going to strip shared libraries"
    for f in `find "${BUILD_INSTALL_PATH}/lib" -type f -name "*.so"`; do
        echo "stripping $f"
        sudo $llvm_exec_path -s $f
    done
fi

if [[ ${IS_CLANG_BUILD_ANALYZER} == ON ]]; then
    echo ${SEPARATION_LINE}
    echo "TODO complete this tool usage"
fi

echo ${SEPARATION_LINE}
popd
