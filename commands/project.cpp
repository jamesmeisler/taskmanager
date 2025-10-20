//
// Created by james on 10/19/25.
//

#include "project.hpp"
namespace schema = db::schema;
ProjectCmdHandles addProjectCommand(CLI::App &app, CLIOptions &opts) {
    ProjectCmdHandles h {};
    h.root = app.add_subcommand("project", "Manage Projects");
    h.add = h.root->add_subcommand("add", "Add a new project");
    h.add->add_option("-n,--name", opts.projectAddOpts.name, "Project name")
        ->required();
    h.list = h.root->add_subcommand("list", "List Projects");
    h.root->require_subcommand(1);
    return h;
}

static void runProjectAdd(const AppContext& ctx, const ProjectAddOpts& opts) {
   //verify name is unique
    auto storage = ctx.storage->getStorage();
    auto projects = storage.get_all<schema::Project>(where(c(&schema::Project::name) == opts.name));
    size_t numProjects = projects.size();
    if (numProjects == 0) {
        storage.insert(schema::Project{-1, opts.name});
    } else {
        throw ProjectNameNotUniqueError("Project already exists");
    }
}

static void runProjectList(const AppContext& ctx, const ProjectListOpts& opts) {
    std::cout << "List projects" << std::endl;
}

void dispatchProject(const ProjectCmdHandles &h,
    const CLI::App &app, const AppContext &ctx, const CLIOptions opts) {
    if (!app.got_subcommand(h.root)) return;
    if (h.root->got_subcommand(h.add)) {
        runProjectAdd(ctx, opts.projectAddOpts);
    }

    if (h.root->got_subcommand(h.list)) {
        runProjectList(ctx, opts.projectListOpts);
    }

    if (h.root->count_all() == 0) throw CLI::CallForHelp();
}


