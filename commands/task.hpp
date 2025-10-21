//
// Created by james on 10/21/25.
//

#ifndef LEARNFTXUI_TASK_HPP
#define LEARNFTXUI_TASK_HPP
#include "../storage.h"
#include "../CLI11.hpp"
#include "../context.hpp"

struct TaskCmdHandles {
    CLI::App* root = nullptr;
    CLI::App* list = nullptr;
    CLI::App* add  = nullptr;
};

TaskCmdHandles addTaskCommand(CLI::App& app, CLIOptions& opts, AppContext& ctx);
#endif //LEARNFTXUI_TASK_HPP