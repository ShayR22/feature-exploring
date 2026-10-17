from conan import ConanFile


class WorkspaceDependencies(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    options = {"build_tests": [True, False], "build_experiments": [True, False]}
    default_options = {"build_tests": True, "build_experiments": True}
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        if self.options.build_experiments:
            self.requires("spdlog/1.11.0")
        if self.options.build_tests:
            self.requires("gtest/1.16.0")
