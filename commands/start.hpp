//
// Created by james on 10/24/25.
//

#ifndef LEARNFTXUI_START_HPP
#define LEARNFTXUI_START_HPP
#pragma once

#include "../CLI11.hpp"
#include "../context.hpp"

struct StartCmdHandles {
    CLI::App* root = nullptr;
};

StartCmdHandles addStartCommand(CLI::App& app, CLIOptions& opts, AppContext& ctx);

void dispatchStart(const StartCmdHandles& h, const CLI::App& app,
    const AppContext& ctx, const CLIOptions& opts);

#endif //LEARNFTXUI_START_HPP