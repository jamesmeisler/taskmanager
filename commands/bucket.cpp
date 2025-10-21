//
// Created by james on 10/20/25.
//

#include "bucket.hpp"
namespace schema = db::schema;

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
