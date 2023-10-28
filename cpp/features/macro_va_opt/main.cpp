/* compile with:
   c++ -std=c++20 main.cpp -o main_va_opt
 */

#include <iostream>
#include <stdio.h>
#include <vector>

#define PARENS ()

// Rescan macro tokens 256 times
#define EXPAND(arg) EXPAND1(EXPAND1(EXPAND1(EXPAND1(arg))))
#define EXPAND1(arg) EXPAND2(EXPAND2(EXPAND2(EXPAND2(arg))))
#define EXPAND2(arg) EXPAND3(EXPAND3(EXPAND3(EXPAND3(arg))))
#define EXPAND3(arg) EXPAND4(EXPAND4(EXPAND4(EXPAND4(arg))))
#define EXPAND4(arg) arg

#define FOR_EACH(macro, ...)                                    \
  __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...)                         \
  macro(a1)                                                     \
  __VA_OPT__(FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define STRINGFY_VARIABLE(name) "_"#name

#define STRINGFY_VARIABLES(...) \
    FOR_EACH(STRINGFY_VARIABLE, __VA_ARGS__)

int main() {
    const char* my_cool_macro_name = STRINGFY_VARIABLES(A, BB, C, DDD);

    printf("my_cool_macro_name: %s\n", my_cool_macro_name);

    return 0;
}
