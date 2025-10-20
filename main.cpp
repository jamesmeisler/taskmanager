//#include "nlohmann/json.hpp"
//#include "ftxui/dom/elements.hpp"
//#include "ftxui/screen/screen.hpp"
#include <iostream>
#include "config.h"
#include "storage.h"
#include "CLI11.hpp"
#include "commands/project.hpp"
#include "context.hpp"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main(const int argc, char** argv) {
    const auto conf = new Config();



    // initialize db
    // first verify that data directory exists:
    try {
        const auto dataPath = conf->ensureAppDataPath();
        auto storage = new Storage(dataPath.string() + "/storage.sqlite");
        storage->sync();

        AppContext ctx{};
        ctx.storage = storage;

        CLIOptions opts{};
        CLI::App app("Task manager.");
        app.require_subcommand(1);

        ProjectAddOpts addOpts;

        //register subcommands
        auto project = addProjectCommand(app, opts);


        CLI11_PARSE(app, argc, argv);

        //dispatch after parse
        dispatchProject(project, app, ctx, opts);

    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "An unknown error occurred.";
    }

    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}