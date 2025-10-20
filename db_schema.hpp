//
// Created by james on 10/19/25.
//

#ifndef LEARNFTXUI_DB_SCHEMA_HPP
#define LEARNFTXUI_DB_SCHEMA_HPP

#pragma once
#include <string>
#include <sqlite_orm/sqlite_orm.h>

using namespace sqlite_orm;

namespace db::schema {
    using std::string;

    struct Project {
        int id;
        string name;
    };

    struct Bucket {
        int id;
        std::unique_ptr<int> projectId;
        string name;
        bool capex;
    };

    struct Task {
        int id;
        std::unique_ptr<int> bucketId;
        string ticketId;
        string description;
    };

    struct Entry {
        int id;
        std::unique_ptr<int> taskId;
        string startTime;
        string endTime;
    };

    inline auto makeEntryTable() {
        return make_table("entry",
            make_column("id", &Entry::id, primary_key()),
            make_column("taskId", &Entry::taskId),
            make_column("startTime", &Entry::startTime),
            make_column("endTime", &Entry::endTime),
            foreign_key(&Entry::taskId).references(&Task::id));
    }

    inline auto makeTaskTable() {
        return make_table("task",
            make_column("id", &Task::id, primary_key()),
            make_column("bucketId", &Task::bucketId),
            make_column("ticketId", &Task::ticketId),
            make_column("description", &Task::description),
            foreign_key(&Task::bucketId).references(&Bucket::id));
    }

    inline auto makeProjectTable() {
        return make_table("project",
            make_column("id", &Project::id, primary_key()),
            make_column("name", &Project::name));
    }

    inline auto makeBucketTable() {
        return make_table("bucket",
            make_column("id", &Bucket::id, primary_key()),
            make_column("projectId", &Bucket::projectId),
            make_column("name", &Bucket::name),
            make_column("capex",  &Bucket::capex),
            foreign_key(&Bucket::projectId).references(&Project::id));
    }

    inline auto makeStorage(const string& dbName) {
        return make_storage(dbName,
            makeProjectTable(),
            makeBucketTable(),
            makeTaskTable(),
            makeEntryTable());
    }

    using DBStorage = decltype(makeStorage(std::declval<const string&>()));
}

#endif //LEARNFTXUI_DB_SCHEMA_HPP