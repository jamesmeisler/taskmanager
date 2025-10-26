//
// Created by james on 10/24/25.
//

#include "start.hpp"
#include "../utils/utils.hpp"

class TaskIdExistsValidator : public CLI::Validator {
public:
    explicit TaskIdExistsValidator(Storage &s)
        : CLI::Validator([&s](const std::string &v) -> std::string {
            if (v.empty()) return "TaskId cannot be empty";

            try {
                auto id = std::stoi(v);
                if (!s.taskIdExists(id)) return "Taskid does not exist";
           return {};
            } catch (...) {
                return "Unrecognized task id";
            }



        }, "taskIdExists") {
        name("TaskIdExistsValidator");
        description("TaskIdExists validator");
    }
};

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
    TaskIdExistsValidator taskIdExistsValidator(*ctx.storage);
    h.root->add_option("-t,--task", opts.startOpts.taskId, "Id of the task to start")
        ->check(taskIdExistsValidator)
        ->required();

    return h;
}
