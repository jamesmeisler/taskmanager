//
// Created by james on 10/24/25.
//

#ifndef LEARNFTXUI_UTILS_HPP
#define LEARNFTXUI_UTILS_HPP
#include <chrono>
#include <string>

using ReportDate = std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::duration<long, std::ratio<86400>>> ;

std::string getDateTime8601();
std::string getDateTime8601(ReportDate date);


#endif //LEARNFTXUI_UTILS_HPP