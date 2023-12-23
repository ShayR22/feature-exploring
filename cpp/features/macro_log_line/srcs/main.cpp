#include <source_location>
#include <cstdio>

enum class LibResults {
    Success,
    InvalidArgument,
    InvalidType,
    InternalError,
};



#define RET_ON_ERR(res, fmt, ...)                                                   \
    do {                                                                            \
        if (res != LibResults::Success) {                                           \
            const std::source_location loc = std::source_location::current();       \
            printf(fmt " file=[%s] func=[%s] line=[%d]\n",                          \
                   ##__VA_ARGS__, loc.file_name(),                                  \
                   loc.function_name(), loc.line());                                \
            return res;                                                             \
        }                                                                           \
    } while(0)


LibResults foo() {
    RET_ON_ERR(LibResults::Success, "gg");
    return LibResults::Success;
}

LibResults bar(LibResults result) {
    int x = 2;
    LibResults res = foo();
    RET_ON_ERR(res, "failed to foo x=[%d]", x);

    x = 4;
    RET_ON_ERR(result, "failed to result x=[%d]", x);
    return LibResults::Success;
}


int main(int argc, const char* argv[]) {
    if (argc % 2 == 0) {
        bar(LibResults::InternalError);
    } else {
        bar(LibResults::Success);
    }


    return 0;
}