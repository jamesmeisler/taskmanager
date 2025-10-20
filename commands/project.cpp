//
// Created by james on 10/19/25.
//
#include <ftxui/dom/elements.hpp>  // for color, Fit, LIGHT, align_right, bold, DOUBLE
#include <ftxui/dom/table.hpp>      // for Table, TableSelection
#include <ftxui/screen/screen.hpp>  // for Screen

#include "ftxui/dom/node.hpp"  // for Render
#include "ftxui/screen/color.hpp"  // for Color, Color::Blue, Color::Cyan, Color::White, ftxui
#include <vector>
#include <algorithm>
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

std::vector<std::vector<std::string>> convertProjectVector(std::vector<schema::Project> src) {
    std::vector<std::vector<std::string>> dst;
    dst.reserve(src.size() + 1);
    dst.emplace_back(std::initializer_list<std::string>{"id", "name"});
    std::ranges::transform(src, std::back_inserter(dst),
                           [](const schema::Project& project) {
                               return std::vector<std::string>{
                                   std::to_string(project.id),
                                   project.name,
                               };
                           });
    return dst;
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
    using namespace ftxui;
    auto storage = ctx.storage->getStorage();
    auto projects = storage.get_all<schema::Project>();

    //Convert all values to strings
    auto converted = convertProjectVector(projects);

    auto table = Table(converted);

    table.SelectAll().Border(LIGHT);
    table.SelectRow(0).Decorate(bold);
    table.SelectRow(0).SeparatorVertical(LIGHT);

    //Render and display the table;
    auto document = table.Render();
    auto screen = Screen::Create(Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
    std::cout << std::endl;
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


