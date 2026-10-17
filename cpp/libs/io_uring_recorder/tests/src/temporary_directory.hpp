#pragma once

#include <cerrno>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <system_error>
#include <vector>

// Own an exclusively created directory. Declare this before recording objects
// so their destruction precedes cleanup, including when the test throws.
class TemporaryDirectory {
public:
    explicit TemporaryDirectory(const std::string& prefix) {
        const auto pattern = (std::filesystem::temp_directory_path() / (prefix + "_XXXXXX")).string();
        std::vector<char> buffer(pattern.begin(), pattern.end());
        buffer.push_back('\0');
        const char* directory = ::mkdtemp(buffer.data());
        if (directory == nullptr) {
            throw std::system_error(errno, std::generic_category(), "mkdtemp");
        }
        _path = directory;
    }

    ~TemporaryDirectory() {
        if (_remove_on_destruction) {
            std::error_code error;
            std::filesystem::remove_all(_path, error);
        }
    }

    TemporaryDirectory(const TemporaryDirectory&) = delete;
    TemporaryDirectory& operator=(const TemporaryDirectory&) = delete;
    TemporaryDirectory(TemporaryDirectory&&) = delete;
    TemporaryDirectory& operator=(TemporaryDirectory&&) = delete;

    [[nodiscard]] const std::filesystem::path& path() const { return _path; }
    void keep() { _remove_on_destruction = false; }

private:
    std::filesystem::path _path;
    bool _remove_on_destruction{true};
};
