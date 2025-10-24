//
// Created by james on 10/20/25.
//

#ifndef LEARNFTXUI_CONTEXT_HPP
#define LEARNFTXUI_CONTEXT_HPP
#pragma once
#include <string>
#include "storage.h"

struct AppContext {
    Storage* storage;
};

struct GlobalOpts {};


struct ProjectListOpts {};
struct ProjectAddOpts {
    std::string name;
};

struct BucketAddOpts {
    std::string name;
    int projectId;
    bool capex;
};

struct TaskAddOpts {
    std::string description;
    std::string ticketId;
    bool start;
    int bucketId;
};

struct StartOpts {
    int taskId;
};

struct CLIOptions {
    GlobalOpts globalOpts{};
    ProjectAddOpts  projectAddOpts{};
    ProjectListOpts  projectListOpts{};
    BucketAddOpts    bucketAddOpts{};
    TaskAddOpts       taskAddOpts{};
    StartOpts         startOpts{};
};

#endif




