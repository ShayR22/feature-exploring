#include <Python.h>
#include <stdio.h>
#include <memory>
#include <filesystem>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

class WCharString {
public:
    WCharString(const std::string& str) {
        _wchar_string = std::make_unique<wchar_t[]>(str.size() + 1);
        mbstowcs(_wchar_string.get(), str.c_str(), str.size());
    }

    const wchar_t* get() {
        return _wchar_string.get();
    }
private:
    std::unique_ptr<wchar_t[]> _wchar_string;
};


int run_python(const std::string& filename) {
    if (!std::filesystem::exists(filename)) {
        printf("%s doesn't exist\n", filename.c_str());
        return -1;
    }

    Py_Initialize();

    WCharString w_filename{filename};
    WCharString w_r_flag{"r"};

	FILE* fp = _Py_wfopen(w_filename.get(), w_r_flag.get());
	int rv = PyRun_SimpleFile(fp, filename.c_str());

    Py_Finalize();
    return rv;
}

int run_python_system(const std::string& filename) {
    if (!std::filesystem::exists(filename)) {
        printf("%s doesn't exist\n", filename.c_str());
        return -1;
    }

    int rv = system(filename.c_str());
    return rv;
}


class Popen {
public:
    struct PopenResult {
        std::string stdout_output;
        int status_code;
    };

    static PopenResult execute(const std::string& command) {
        PopenResult res{};

        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            printf("popen() failed!\n");
            res.status_code = -1;
            return res;
        }

        std::array<char, 256> buffer;
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            res.stdout_output += buffer.data();
        }

        res.status_code = WEXITSTATUS(pclose(pipe));
        return res;
    }
};

int run_python_popen(const std::string &filename) {
    // std::array<char, 256> buffer;
    // std::string result;
    // std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(filename.c_str(), "r"), pclose);
    // if (!pipe) {
    //     printf("popen() failed!");
    //     return -1;
    // }

    // while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    //     result += buffer.data();
    // }

    // printf("process stdout:\n%s\n", result.c_str());

    // return 0;

    Popen::PopenResult res = Popen::execute(filename);
    printf("status code: %d\n", res.status_code);
    printf("stdout: %s\n", res.stdout_output.c_str());
    return res.status_code;
}


int main(int argc, const char* argv[]) {
    std::string python_to_run{"test.py"};

    if (argc < 2) {
        printf("please add run mode (1/2/3 : system/PyRun/popen), optional argumnet script path");
        return -1;
    }

    int mode = atoi(argv[1]);
    if (mode != 1 && mode != 2 && mode != 3) {
        printf("invalid mode %d, please enter 1/2\n", mode);
        return -1;
    }

    if (argc > 2) {
        python_to_run = argv[2];
    }

    int rv = -1;
    if (mode == 1) {
        rv = run_python_system(python_to_run);
    } else if (mode == 2) {
        rv = run_python(python_to_run);
    } else {
        rv = run_python_popen(python_to_run);
    }

    printf("return value = %d\n", rv);

    return rv;
}