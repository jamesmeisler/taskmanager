//
// Created by james on 10/21/25.
//
//
// Created by james on 10/19/25.
//
#include <ftxui/dom/elements.hpp>  // for color, Fit, LIGHT, align_right, bold, DOUBLE
#include <ftxui/dom/table.hpp>      // for Table, TableSelection
#include <ftxui/screen/screen.hpp>  // for Screen

#include "ftxui/dom/node.hpp"  // for Render

#include "task.hpp"
#include <chrono>
#include <format>

namespace schema = db::schema;
struct ActiveTaskVM {
    int id;
    std::string description;
    std::string ticketId;
    std::string bucket;
    double hours;
};

struct TaskVM {
    int id;
    std::string description;
    std::string ticketId;
    std::string bucket;
};

inline std::string getDateTime8601() {
    auto now = floor<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto out = std::format("{:%FT%TZ}", now);
    return out;
}

static ftxui::Table styleActiveTable(ftxui::Table table, const std::vector<std::vector<std::string>>& data) {
    using namespace ftxui;
    for (int i = 1; i < static_cast<int>(data.size()); i++) {
        if (std::stoi(data[i][4]) > 8) {
            table.SelectCell(4, i).Decorate(color(Color::Red));
            table.SelectCell(4, i).Decorate(bold);
        }
    }

    return table;
}

void printTable(const std::vector<std::vector<std::string>>& data, const std::string& title) {
    using namespace ftxui;
    auto table = Table(data);

    if (title == "Active Tasks") {
        table = styleActiveTable(table, data);
    }

    table.SelectAll().Border(LIGHT);

    //style the table
    table.SelectAll().Border(LIGHT);
    table.SelectAll().SeparatorVertical(LIGHT);
    table.SelectRow(0).Border(LIGHT);
    table.SelectRow(0).DecorateCells(bold);
    table.SelectRow(0).DecorateCells(color(Color::White));
    table.SelectAll().DecorateCells(size(WIDTH, GREATER_THAN, 3));



    auto tableEl = table.Render();
    auto document = vbox({
        text(title + ":") | bold,
        separator(),
        tableEl
    });

    auto screen = Screen::Create(Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
    std::cout << std::endl;
}

std::vector<std::vector<std::string>> convertActiveVector(std::vector<ActiveTaskVM> src) {
    std::vector<std::vector<std::string>> dest;
    dest.reserve(src.size() + 1);
    dest.emplace_back(std::initializer_list<std::string>{"id", "description", "ticket id", "bucket", "elapsed hours"});
    std::ranges::transform(src, std::back_inserter(dest), [](const ActiveTaskVM& vm) {
        std::ostringstream out;
        out << std::fixed << std::setprecision(2) << vm.hours;
        std::string hours = out.str();

        return std::vector<std::string>{
            std::to_string(vm.id),
            vm.description,
            vm.ticketId,
            vm.bucket,
            hours
        };
    });
    return dest;
}

std::vector<std::vector<std::string>> convertTaskVector(std::vector<TaskVM>& src) {
    std::vector<std::vector<std::string>> dest;
    dest.reserve(src.size() + 1);
    dest.emplace_back(std::initializer_list<std::string>{"id", "description", "ticket id", "bucket"});
    std::ranges::transform(src, std::back_inserter(dest), [](const TaskVM& vm) {
        return std::vector<std::string>{
        std::to_string(vm.id),
        vm.description,
        vm.ticketId,
        vm.bucket
        };
    });
    return dest;
}

std::chrono::sys_time<std::chrono::milliseconds> parse8601( std::string& stringDate) {
    std::istringstream iss(stringDate);
    std::string save;
    iss >> save;
    std::istringstream in{save};
    std::chrono::sys_time<std::chrono::milliseconds> tp;
    in >> std::chrono::parse("%FT%TZ", tp);
    if (in.fail()) {
        in.clear();
        in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        in.str(save);
        in >> std::chrono::parse("%FT%T%Ez", tp);
    }
    return tp;
}

double getElapsedHours(std::string& startTime) {
    const auto now = floor<std::chrono::milliseconds>(std::chrono::system_clock::now());
    const auto start = parse8601(startTime);
    const auto elapsed = now - start;
    const auto elapsedHours = std::chrono::duration<double, std::chrono::hours::period>(elapsed).count();
    return elapsedHours;

}

static void runTaskStop(const AppContext& ctx) {
    using namespace db::schema;
    auto storage = ctx.storage->getStorage();
    auto endTime = getDateTime8601();
    storage.update_all(set(c(&Entry::endTime) = endTime), where(c(&Entry::endTime) == ""));
}

static void runTaskList(const AppContext& ctx, const bool active = false) {
    using namespace db::schema;
    auto title = active ? "Active Tasks" : "Tasks";
    auto storage = ctx.storage->getStorage();
    if (active) {
        const auto rows = storage.select(columns(&Task::id, &Task::ticketId,
        &Task::description, &Bucket::name, &Entry::startTime),
        inner_join<Bucket>(on(c(&Task::bucketId) == &Bucket::id)),
        inner_join<Entry>(on(&Entry::taskId == c(&Task::id))), where(c(&Entry::endTime) == ""));

        std::vector<ActiveTaskVM> vms;
        vms.reserve(rows.size());
        for (const auto& row : rows) {
            auto start = std::get<4>(row);
            vms.push_back(ActiveTaskVM {
                std::get<0>(row),
                std::get<2>(row),
                std::get<1>(row),
                std::get<3>(row),
                getElapsedHours(start)
            });
        }

        if (vms.empty()) {
            std::cout << "No active tasks.\n";
            return;
        }

        const auto converted = convertActiveVector(vms);
        printTable(converted, title);
    } else {
        const auto rows = storage.select(columns(&Task::id, &Task::ticketId,
            &Task::description, &Bucket::name),
            inner_join<Bucket>(on(c(&Task::bucketId) == &Bucket::id)));
        std::vector<TaskVM> vms;
        vms.reserve(rows.size());
        for (const auto& row : rows) {
            vms.push_back(TaskVM {
                std::get<0>(row),
                std::get<2>(row),
                std::get<1>(row),
                std::get<3>(row)
            });
        }
        const auto converted = convertTaskVector(vms);
        printTable(converted, title);
    }


}

static void runTaskAdd(const AppContext &ctx, const TaskAddOpts &opts) {
    auto storage = ctx.storage->getStorage();
    const auto taskId = storage.insert(schema::Task{-1, opts.bucketId, opts.ticketId, opts.description});

    if (opts.start) {
        storage.insert(schema::Entry{-1,  taskId, getDateTime8601()});
    }
}

TaskCmdHandles addTaskCommand(CLI::App &app, CLIOptions &opts, AppContext &ctx) {
    TaskCmdHandles h;
    h.root = app.add_subcommand("task", "Manage Tasks");

    h.add = h.root->add_subcommand("add", "Add Tasks");
    h.add->add_option("-d,--desc", opts.taskAddOpts.description, "Set task description.");
    h.add->add_option("-j,--jira", opts.taskAddOpts.ticketId, "Set Jira ticket.");
    h.add->add_option("-b,--bucket", opts.taskAddOpts.bucketId, "Set bucket id.");
    h.add->add_flag("--start,-s", opts.taskAddOpts.start, "Start task on creation");

    h.list = h.root->add_subcommand("list", "List Tasks");
    h.active = h.root->add_subcommand("active", "List active Tasks");
    h.stop = h.root->add_subcommand("stop", "Stop  active tasks");
    h.root->require_subcommand(1);
    return h;
}

void dispatchTask(const TaskCmdHandles &h,
    const CLI::App &app, const AppContext &ctx, const CLIOptions &opts) {
    if (!app.got_subcommand(h.root)) return;
    if (h.root->got_subcommand(h.add)) {
        runTaskAdd(ctx, opts.taskAddOpts);
    }

    if (h.root->got_subcommand(h.list)) {
        runTaskList(ctx, false);
    }

    if (h.root->got_subcommand(h.active)) {
        runTaskList(ctx, true);
    }

    if (h.root->got_subcommand(h.stop)) {
        runTaskStop(ctx);
    }
    if (h.root->count_all() == 0) throw CLI::CallForHelp();
}

