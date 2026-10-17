#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../scripts/output.sh"
COMPOSE_FILE="$SCRIPT_DIR/docker-compose.yaml"
COMPOSE_PROJECT="feature-exploring-cpp"

print_usage() {
    cat <<'EOF'
Usage: ./cpp/setup/install.sh [command]

Build and start the C++ development container. Run as your normal host user
so generated files and mounted caches keep your user/group ownership.

Commands (choose one):
  --all          Build the image using cached layers, then start/update the
                 container. This is the default when no command is supplied.
  --build        Build the image using cached layers. Do not start or restart
                 the container.
  --rebuild      Build the image without cached layers, then start/update the
                 container.
  --restart-env  Build the image if needed and forcibly recreate/start the
                 container. Preserve the repository and mounted caches.
  --help, -h     Show this help without changing the environment.

Host prerequisites:
  Bash, Docker with Compose and Buildx plugins, and access to the Docker daemon
  as your normal user. C++ tools, CMake and Conan are installed in the image.
  Ubuntu: https://docs.docker.com/engine/install/ubuntu/
  WSL2:   https://docs.docker.com/desktop/features/wsl/
  Access: https://docs.docker.com/engine/install/linux-postinstall/

Environment overrides:
  NO_COLOR         Disable colored script output.
  CLICOLOR_FORCE   Set to 1 to color output even when redirected.
  CONTAINER_NAME    Container name (default: cpp_builder).
  CONAN_CACHE_DIR   Host Conan cache (default:
                    $HOME/.cache/feature-exploring/conan2).
  CCACHE_CACHE_DIR  Host compiler cache (default:
                    $HOME/.cache/feature-exploring/ccache).

The repository is mounted at /workspace. DEV_UID and DEV_GID are taken from
id -u and id -g and passed as Docker image build arguments.

Examples:
  ./cpp/setup/install.sh --all
  ./cpp/setup/install.sh --build
  ./cpp/setup/install.sh --restart-env
  CONTAINER_NAME=my_cpp_builder ./cpp/setup/install.sh --all
EOF
}

print_prerequisite_guidance() {
    cat >&2 <<'EOF'
Install Docker with the Compose and Buildx plugins, then ensure Docker is
running and accessible as your normal user. Run this script without sudo
so generated files and caches match your user/group ownership.
  Ubuntu: https://docs.docker.com/engine/install/ubuntu/
  WSL2:   https://docs.docker.com/desktop/features/wsl/
  Access: https://docs.docker.com/engine/install/linux-postinstall/
EOF
}

check_prerequisites() {
    if ! command -v docker >/dev/null 2>&1; then
        print_error "Docker is not installed or is not on PATH."
        print_prerequisite_guidance
        return 1
    fi
    if ! docker compose version >/dev/null 2>&1; then
        print_error "The Docker Compose plugin is unavailable (docker compose)."
        print_prerequisite_guidance
        return 1
    fi
    if ! docker buildx version >/dev/null 2>&1; then
        print_error "The Docker Buildx plugin is unavailable (docker buildx)."
        print_prerequisite_guidance
        return 1
    fi
    local docker_error
    if ! docker_error="$(docker info 2>&1)"; then
        print_error "Cannot access the Docker daemon. Check that Docker is running and your user has access."
        printf '%s\n' "$docker_error" >&2
        print_prerequisite_guidance
        return 1
    fi
}

configure_environment() {
    export WORKSPACE_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
    export DEV_UID="$(id -u)"
    export DEV_GID="$(id -g)"
    export CONTAINER_NAME="${CONTAINER_NAME:-cpp_builder}"
    export CONAN_CACHE_DIR="${CONAN_CACHE_DIR:-$HOME/.cache/feature-exploring/conan2}"
    export CCACHE_CACHE_DIR="${CCACHE_CACHE_DIR:-$HOME/.cache/feature-exploring/ccache}"

    mkdir -p "$CONAN_CACHE_DIR" "$CCACHE_CACHE_DIR"
}

compose() {
    docker compose --project-name "$COMPOSE_PROJECT" -f "$COMPOSE_FILE" "$@"
}

build_image() {
    print_step "Docker image build" "Building the C++ development image"
    compose build "$@"
}

start_environment() {
    print_step "Container startup" "Starting/updating container '$CONTAINER_NAME'"
    compose up -d
}

restart_environment() {
    print_step "Recreate container" "Recreating container '$CONTAINER_NAME', preserving mounted files"
    compose up --build --force-recreate -d
}

main() {
    if (( $# > 1 )); then
        echo "Choose one command; received $# arguments." >&2
        print_usage >&2
        return 2
    fi

    local action="${1:---all}"
    case "$action" in
        --help|-h)
            print_usage
            return 0
            ;;
        --all|--build|--rebuild|--restart-env)
            ;;
        *)
            echo "Unknown command: $action" >&2
            print_usage >&2
            return 2
            ;;
    esac

    check_prerequisites
    configure_environment

    case "$action" in
        --all)
            build_image
            start_environment
            ;;
        --build)
            build_image
            ;;
        --rebuild)
            build_image --no-cache
            start_environment
            ;;
        --restart-env)
            restart_environment
            ;;
    esac
    print_success "Development environment setup completed."
}

main "$@"
