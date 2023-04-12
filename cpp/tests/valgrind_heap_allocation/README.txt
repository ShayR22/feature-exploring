heap profiling is done in valgrind massif tool:


following steps:
- compile:
    -g is importent for symbols
    g++ -std=c++20 -O2 -g main.cpp

- clean envrionement from previous runs
    rm mas* output.log

- run program with valgrind
    --time-unit=ms to try to be "realtime"
    --stacks to profile the stack
    program and arguments in this case 1MB of allocation and 10 seconds of run

    valgrind --time-unit=ms --stacks=yes --tool=massif ./a.out 1048576 10

- interpert the results for something less CPUish
    output.log would contain snapshot results
    ms_print massif.out.* > output.log

