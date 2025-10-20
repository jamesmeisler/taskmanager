//
// Created by james on 10/19/25.
//

#ifndef LEARNFTXUI_PROJECT_H
#define LEARNFTXUI_PROJECT_H
#pragma once

#include "../storage.h"
#include "../CLI11.hpp"
#include "../context.hpp"

struct ProjectNameNotUniqueError final : CLI::ParseError {
    explicit ProjectNameNotUniqueError(std::string msg, int code = 10)
        : CLI::ParseError(msg.c_str(), code) {}
};

struct ProjectCmdHandles {
    CLI::App* root = nullptr;
    CLI::App* list = nullptr;
    CLI::App* add  = nullptr;
};

ProjectCmdHandles addProjectCommand(CLI::App& app, CLIOptions& opts);

void dispatchProject(const ProjectCmdHandles& h, const CLI::App& app,
    const AppContext& ctx, const CLIOptions opts);
#endif //LEARNFTXUI_PROJECT_H