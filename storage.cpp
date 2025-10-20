//
// Created by james on 10/19/25.
//

#include "storage.h"
#include "db_schema.hpp"

Storage::Storage(const std::string &dbName)
    : storage(db::schema::makeStorage(dbName)) {}

void Storage::sync() {
    storage.sync_schema();
}


