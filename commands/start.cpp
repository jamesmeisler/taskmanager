//
// Created by james on 10/24/25.
//

#include "start.hpp"

#include <vector>
#include "../utils/utils.hpp"

static void runStart(const AppContext& ctx, const StartOpts& opts) {
    using namespace db::schema;
    auto storage = ctx.storage->getStorage();
    storage.insert(Entry{-1, opts.taskId, getDateTime8601(), ""});
}

void dispatchStart(const StartCmdHandles& h, const CLI::App& app,
    const AppContext& ctx, const CLIOptions& opts) {
    if (!app.got_subcommand(h.root)) return;
    runStart(ctx, opts.startOpts);
}

StartCmdHandles addStartCommand(CLI::App& app, CLIOptions& opts, AppContext& ctx) {
    StartCmdHandles h {};
    h.root = app.add_subcommand("start", "Start working on a task");
    h.root->add_option("-t,--task", opts.startOpts.taskId, "Id of the task to start")
    ->required();

    return h;
}
