
#pragma once

#include "../cli11/CLI11.hpp"
#include <string>
#include <cstdio>

class CliRunner {
public:
    static int run(CLI::App& app, int argc, const char* const* argv) {
        // run the CLI
        try {
            app.parse(argc, argv);

            // if no subcommands given display help message
            if (app.get_subcommands().size() == 0 ){
                printf("%s\n", app.help().c_str());
            }
        } catch (const CLI::ParseError& e) {
            return app.exit(e);
        }

        return 0;
    }
private:
    CliRunner() = delete;
};