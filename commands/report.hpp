//
// Created by james on 10/26/25.
//

#ifndef LEARNFTXUI_REPORT_HPP
#define LEARNFTXUI_REPORT_HPP
#include "../CLI11.hpp"
#include "../context.hpp"

class WeeklyReportVM {
private:
    std::string projectName;
    std::string bucketName;
    std::string taskDescription;
    std::string ticketId;
    std::string startTime;
    std::string endTime;
    double hoursSpent;


public:
    WeeklyReportVM(const std::string &projectName, const std::string &bucketName, const std::string &taskDescription,
        const std::string &ticketId, const std::string &startTime, const std::string &endTime, const double hoursSpent)
        : projectName(projectName), bucketName(bucketName), taskDescription(taskDescription),
    ticketId(ticketId), startTime(startTime), endTime(endTime), hoursSpent(hoursSpent) {}
};

struct ReportCmdHandles {
    CLI::App* root = nullptr;
};

ReportCmdHandles addReportCommand(CLI::App& app, CLIOptions& opts, AppContext& ctx);

void dispatchReport(const ReportCmdHandles& h, const CLI::App& app,
    const AppContext& ctx, const CLIOptions& opts);
#endif //LEARNFTXUI_REPORT_HPP