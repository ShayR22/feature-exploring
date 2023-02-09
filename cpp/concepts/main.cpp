#include <stdio.h>
#include <string>
#include <concepts>
#include "spdlog/spdlog.h"



// ==============================================================================
// ========================= FOLD EXPRESSION VARIADIC ===========================
// ==============================================================================

static int fold_expression_variadic_i = 1;
static int f_print_i = 1;

template<typename ...Args>
concept MoreThanOneElement = requires(Args ... args) {
    requires sizeof...(args) > 1;
};

template<typename T>
void f_print(const T& element) {
    spdlog::info("{}: {}", f_print_i++, element);
}

template<typename... Args>
requires MoreThanOneElement<Args...>
void f_print(Args&&... args) {
    printf("fold_expression_variadic_i=%d\n", fold_expression_variadic_i++);
    // cpp17 fold expression syntax (avoid recursive calls), args is like a pointer va_list,
    // the entire expresion is inside a callable '()' and ... advance
    // the args to the next argument until no args are left
    (f_print(args), ...);
}


// ==============================================================================
// ========================= RECURSIVE EXPRESSION VARIADIC ======================
// ==============================================================================

static int recursive_variadic_i = 1;
static int r_print_i = 1;

template<typename T>
void r_print(const T& element) {
    spdlog::info("{}: {}", r_print_i++, element);
}

// cant use move semantics in order to enforce 0 types (const & instead of &&)
template<typename FirstArg, typename... RestArgs>
void r_print(const FirstArg& arg, const RestArgs&... args) {
    printf("recursive_variadic_i=%d\n", recursive_variadic_i++);
    r_print(arg);
    r_print(args...);
}

// ==============================================================================
// ================================== MAIN ======================================
// ==============================================================================


int main() {
    f_print(42, 1, 3, "gg", std::string("wp"));
    printf("========================================\n");
    r_print(42, 1, 3, "gg", std::string("wp"));
    return 0;
}