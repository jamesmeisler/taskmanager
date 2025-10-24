//
// Created by james on 10/21/25.
//

#ifndef LEARNFTXUI_TASK_HPP
#define LEARNFTXUI_TASK_HPP
#include "../CLI11.hpp"
#include "../context.hpp"

struct TaskCmdHandles {
    CLI::App* root = nullptr;
    CLI::App* list = nullptr;
    CLI::App* add  = nullptr;
    CLI::App* active = nullptr;
    CLI::App* stop = nullptr;
};

TaskCmdHandles addTaskCommand(CLI::App& app, CLIOptions& opts, AppContext& ctx);
void dispatchTask(const TaskCmdHandles &h,
    const CLI::App& app, const AppContext &ctx, const CLIOptions &opts);
#endif //LEARNFTXUI_TASK_HPP