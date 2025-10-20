//
// Created by james on 10/19/25.
//

#ifndef LEARNFTXUI_STORAGE_H
#define LEARNFTXUI_STORAGE_H
#pragma once
#include <string>
#include "db_schema.hpp"

class Storage {
private:
    db::schema::DBStorage storage;;
public:
    db::schema::DBStorage &getStorage() { return storage; }
    explicit Storage(const std::string &dbName);
    void sync();
};


#endif //LEARNFTXUI_STORAGE_H