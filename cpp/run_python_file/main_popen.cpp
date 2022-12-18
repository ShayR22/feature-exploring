#include <stdio.h>
#include <memory>
#include <filesystem>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>

class Popen {
public:
    struct PopenResult {
        std::string stdout_output;
        int status_code;
    };

    static PopenResult execute(const std::string& python_script, const std::vector<std::string>& args) {
        PopenResult res{};
        if (!std::filesystem::exists(python_script)) {
            printf("%s script doesn't exist\n", python_script.c_str());
            res.status_code = -1;
            return res;
        }
        printf("%s script found on filesystem\n", python_script.c_str());

        std::string command = python_script;
        for (const auto& arg: args) {
            command += " " + arg;
        }

        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            printf("popen() failed!\n");
            res.status_code = -1;
            return res;
        }
        printf("success at opening pipe command=%s\n", command.c_str());

        std::array<char, 256> buffer;
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            res.stdout_output += buffer.data();
        }

        res.status_code = WEXITSTATUS(pclose(pipe));
        printf("%s status code: %d\n", command.c_str(), res.status_code);
        return res;
    }
};


int main(int argc, const char* argv[]) {
    std::string python_to_run{"./test.py"};
    std::vector<std::string> args;
    if (argc > 1) {
        python_to_run = argv[1];
    }

    for (int i = 2; i < argc; i++) {
        args.push_back(argv[i]);
    }

    Popen::PopenResult res = Popen::execute(python_to_run, args);
    const char* stdout_capture = res.stdout_output.empty() ? "empty" : res.stdout_output.c_str();

    printf("status code: %d\n", res.status_code);
    printf("stdout: %s\n", stdout_capture);

    return res.status_code;
}