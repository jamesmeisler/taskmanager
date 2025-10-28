//
// Created by james on 10/24/25.
//

#include "utils.hpp"
#include <chrono>
#include <format>
#include <string>

std::string getDateTime8601() {
    auto now = floor<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto out = std::format("{:%FT%TZ}", now);
    return out;
}

std::string getDateTime8601(ReportDate date) {
    return std::format("{:%FT%TZ}", date);
}
