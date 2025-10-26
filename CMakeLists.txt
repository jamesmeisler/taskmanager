cmake_minimum_required(VERSION 4.0)
project(learnFTXUI)

set(CMAKE_CXX_STANDARD 20)

add_executable(learnFTXUI main.cpp
        storage.cpp
        storage.h
        db_schema.hpp
        config.cpp
        config.h
        commands/project.cpp
        commands/project.hpp
        context.hpp
        commands/bucket.cpp
        commands/bucket.hpp
        commands/task.cpp
        commands/task.hpp
        commands/start.hpp
        commands/start.cpp
        utils/utils.cpp
        utils/utils.hpp
        commands/report.cpp
        commands/report.hpp)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(learnFTXUI PRIVATE nlohmann_json::nlohmann_json)

find_package(ftxui CONFIG REQUIRED)
target_link_libraries(learnFTXUI PRIVATE ftxui::dom ftxui::screen ftxui::component)

find_package(SqliteOrm CONFIG REQUIRED)
target_link_libraries(learnFTXUI PRIVATE sqlite_orm::sqlite_orm)

find_package(date CONFIG REQUIRED)
target_link_libraries(learnFTXUI PRIVATE date::date date::date-tz)