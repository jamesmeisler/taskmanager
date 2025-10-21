//
// Created by james on 10/20/25.
//

#ifndef LEARNFTXUI_BUCKET_HPP
#define LEARNFTXUI_BUCKET_HPP
#pragma once

#include "../CLI11.hpp"
#include "../context.hpp"

struct BucketNameNotUniqueError final : CLI::ParseError {
    explicit BucketNameNotUniqueError(std::string msg, int code = 11)
        : CLI::ParseError(msg.c_str(), code) {}
};

struct BucketCmdHandles {
    CLI::App* root = nullptr;
    CLI::App* list = nullptr;
    CLI::App* add = nullptr;
};

BucketCmdHandles addBucketCommand(CLI::App& app, CLIOptions& opts, AppContext& ctx);

void dispatchBucket(const BucketCmdHandles& h, const CLI::App& app,
    const AppContext& ctx, const CLIOptions opts);

#endif //LEARNFTXUI_BUCKET_HPP