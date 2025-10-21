//
// Created by james on 10/20/25.
//
#include <ftxui/dom/elements.hpp>  // for color, Fit, LIGHT, align_right, bold, DOUBLE
#include <ftxui/dom/table.hpp>      // for Table, TableSelection
#include <ftxui/screen/screen.hpp>  // for Screen

#include "ftxui/dom/node.hpp"  // for Render
#include "ftxui/screen/color.hpp"  // for Color, Color::Blue, Color::Cyan, Color::White, ftxui
#include <vector>
#include <algorithm>

#include "bucket.hpp"
namespace schema = db::schema;

struct BucketVM {
    int id;
    std::string name;
    std::string projectName;
    std::string capex;
};

std::vector<std::vector<std::string>> convertBucketVector(std::vector<BucketVM> src) {
    std::vector<std::vector<std::string>> dst;
    dst.reserve(src.size() + 1);
    dst.emplace_back(std::initializer_list<std::string>{"id", "name", "project name", "capx"});
    std::ranges::transform(src, std::back_inserter(dst),
        [](BucketVM bucket) {
            return std::vector{
            std::to_string(bucket.id),
            bucket.name,
            bucket.projectName,
            bucket.capex};
        });
    return dst;
}

class BucketNameUniqueValidator : public CLI::Validator {
public:
    explicit BucketNameUniqueValidator(Storage& s)
        : CLI::Validator([&s](const std::string &v) -> std::string  {
            if (v.empty()) return "Bucket name cannot be empty";
            if (s.bucketNameExists(v)) return "Duplicate bucket name: " + v;
            return {};
        }, "uniqueBucketName") {
        name("BucketNameUniqueValidator");
        description("Bucket name must be unique in DB");
    }
};

class ProjectIdExistsValidator : public CLI::Validator {
public:
    explicit ProjectIdExistsValidator(Storage& s)
        : CLI::Validator([&s](const std::string &v) -> std::string {
            if (v.empty()) return "Project Id cannot be empty";
            if (!s.projectIdExists(atoi(v.c_str()))) return "No projects with Id: " + v;
            return {};
        }, "projectExits") {
        name("ProjectIdExistsValidator");
        description("Project Id must exist in database");
    }
};

BucketCmdHandles addBucketCommand(CLI::App &app, CLIOptions &opts, AppContext& ctx) {
    BucketCmdHandles h {};
    h.root = app.add_subcommand("bucket", "Manage Buckets");
    h.add = h.root->add_subcommand("add", "Add Bucket");

    BucketNameUniqueValidator unique(*ctx.storage);
    h.add->add_option("-n,--name", opts.bucketAddOpts.name, "Bucket name")
        ->check(unique)
        ->required();

    ProjectIdExistsValidator projectIdExists(*ctx.storage);
    h.add->add_option("--project,-p", opts.bucketAddOpts.projectId, "Project Id")
        ->check(projectIdExists)
        ->required();

    h.add->add_flag("-c,--capex", opts.bucketAddOpts.capex, "Bucket is Capex");

    h.list = h.root->add_subcommand("list", "List Buckets");

    h.root->require_subcommand(1);
    return h;
}

static void runBucketList(const AppContext& ctx) {
    using namespace ftxui;
    auto storage = ctx.storage->getStorage();
    auto rows = storage.select(columns(&schema::Bucket::id,
        &schema::Bucket::name, &schema::Project::name, &schema::Bucket::capex),
        inner_join<schema::Project>(on(c(&schema::Bucket::projectId) == &schema::Project::id)));

    std::vector<BucketVM> vms;
    vms.reserve(rows.size());
    for (const auto& row : rows) {
        vms.push_back(BucketVM {
        std::get<0>(row),
        std::get<1>(row),
        std::get<2>(row),
        std::get<3>(row) ? "yes" : "no"
        });
    }

    auto converted = convertBucketVector(vms);

    auto table = Table(converted);

    //style the table
    table.SelectAll().Border(LIGHT);
    table.SelectAll().SeparatorVertical(LIGHT);
    table.SelectRow(0).Border(LIGHT);
    table.SelectRow(0).DecorateCells(bold);
    table.SelectRow(0).DecorateCells(color(Color::White));
    table.SelectAll().DecorateCells(size(WIDTH, GREATER_THAN, 3));

    auto tableEl = table.Render();
    auto document = vbox({
        text("Buckets:") | bold,
        separator(),
        tableEl,

    });

    auto screen = Screen::Create(Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
    std::cout << std::endl;
}


static void runBucketAdd(const AppContext& ctx, const BucketAddOpts& opts) {
    try {
        auto storage = ctx.storage->getStorage();
        storage.insert(schema::Bucket{-1, opts.projectId, opts.name, opts.capex });
    } catch (...) {
    }

}

void dispatchBucket(const BucketCmdHandles &h, const CLI::App &app,
                    const AppContext &ctx, const CLIOptions opts) {
    if (!app.got_subcommand(h.root)) return;
    if (h.root->got_subcommand(h.list)) runBucketList(ctx);
    if (h.root->got_subcommand(h.add)) runBucketAdd(ctx, opts.bucketAddOpts);
    if (h.root->count_all() == 0) throw CLI::CallForHelp();
}
