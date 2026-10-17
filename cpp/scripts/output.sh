#!/usr/bin/env bash
# Shared terminal output for the host setup and container build scripts.

print_message() {
    local color="$1"
    shift
    if [[ ! -v NO_COLOR && ( -t 1 || "${CLICOLOR_FORCE:-0}" != 0 ) ]]; then
        printf '\033[%sm%s\033[0m\n' "$color" "$*"
    else
        printf '%s\n' "$*"
    fi
}

print_step() {
    if [[ -v OUTPUT_STEP_START ]]; then
        OUTPUT_STEP_TIMES+=("$(( SECONDS - OUTPUT_STEP_START ))")
    else
        OUTPUT_TOTAL_START=$SECONDS
        OUTPUT_STEP_NAMES=()
        OUTPUT_STEP_TIMES=()
        trap 'print_time_summary "$?"' EXIT
    fi
    OUTPUT_STEP_NAMES+=("$1")
    OUTPUT_STEP_START=$SECONDS
    print_message '1;33' "----- [${#OUTPUT_STEP_NAMES[@]}] ${2:-$1} -----"
}

print_time_summary() {
    local status="$1" index row
    OUTPUT_STEP_TIMES+=("$(( SECONDS - OUTPUT_STEP_START ))")
    printf '\n'
    print_message '1;34' '============== TIME SUMMARY =============='
    for (( index=0; index<${#OUTPUT_STEP_NAMES[@]}; ++index )); do
        printf '%-20s %ss\n' "${OUTPUT_STEP_NAMES[index]}:" "${OUTPUT_STEP_TIMES[index]}"
    done
    printf '%s\n' '------------------------------------------'
    printf -v row 'Total time:          %ss' "$(( SECONDS - OUTPUT_TOTAL_START ))"
    if (( status == 0 )); then
        print_success "$row"
    else
        print_error "$row (failed, exit $status)"
    fi
    print_message '1;34' '=========================================='
}

print_success() {
    print_message '1;32' "$*"
}

print_error() {
    print_message '1;31' "$*" >&2
}
