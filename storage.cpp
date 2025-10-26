//
// Created by james on 10/19/25.
//

#include "storage.h"
#include "db_schema.hpp"

namespace schema = db::schema;

Storage::Storage(const std::string &dbName)
    : storage(db::schema::makeStorage(dbName)) {}

void Storage::sync() {
    storage.sync_schema();
}

bool Storage::bucketNameExists(const std::string &bucketName) {
    const auto buckets =
        storage.get_all<schema::Bucket>(where(c(&schema::Bucket::name) == bucketName));

    return buckets.size() > 0;
}

bool Storage::projectNameExists(const std::string &projectName) {
    const auto projects =
        storage.get_all<schema::Project>(where(c(&schema::Project::name) == projectName));
    return projects.size() > 0;
}

bool Storage::projectIdExists(const int id) {
    if (auto project = storage.get_pointer<schema::Project>(id)) {
        return true;
    }
    return false;
}

bool Storage::taskIdExists(const int id) {
    if (auto task = storage.get_pointer<schema::Task>(id)) {
        return true;
    }
    return false;
}


