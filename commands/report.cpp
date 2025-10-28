//
// Created by james on 10/26/25.
//

#include "report.hpp"
#include <chrono>

#include "../utils/utils.hpp"

using ReportDate = std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::duration<long, std::ratio<86400>>> ;
using ReportDateRange = std::tuple<ReportDate, ReportDate>;

ReportDateRange establishDateRange() {
    using namespace std::chrono;
    auto today = floor<days>(system_clock::now());
    auto monday = today - ((weekday{today} - Monday + days{7}) % days{7});
    auto  friday = monday + (Friday - Monday + days{7}) % days{7};
    std::cout << "Now: " << today << std::endl;
    std::cout << "Monday: " << monday << std::endl;
    std::cout << "Friday: " << friday << std::endl;
    return std::make_tuple(monday, friday);
}

ReportCmdHandles addReportCommand(CLI::App &app, CLIOptions &opts, AppContext &ctx) {
    ReportCmdHandles h {};
    h.root = app.add_subcommand("report", "Generate a weekly report");

    return h;
}

void buildWeeklyReport(const AppContext &ctx, const ReportOpts &opts) {
    using namespace db::schema;
    using namespace sqlite_orm;
    auto range = establishDateRange();
    auto startDate = getDateTime8601(std::get<0>(range));
    auto endDate = getDateTime8601(std::get<1>(range));
    auto storage = ctx.storage->getStorage();
    auto rows = storage.select(columns(&Project::name, &Bucket::name,
        &Task::description, &Task::ticketId, &Entry::startTime, &Entry::endTime, (julianday(&Entry::endTime) - julianday(&Entry::startTime)) * 24),
            left_join<Bucket>(on(&Bucket::projectId == c(&Project::id))),
            left_join<Task>(on(&Task::bucketId == c(&Bucket::id))),
            left_join<Entry>(on(&Entry::taskId == c(&Task::id))),
            where(
                length(&Entry::endTime) > 0
                && datetime(&Entry::startTime) >= datetime(startDate)
                && c(datetime(&Entry::endTime)) <= datetime(endDate)));

    for (const auto &row : rows) {
        std::cout << std::get<0>(row) << std::endl;
        std::cout << std::get<4>(row) << std::endl;
        std::cout << std::get<5>(row) << std::endl;
        std::cout << std::get<6>(row) << std::endl;
    }

}

void dispatchReport(const ReportCmdHandles &h, const CLI::App &app,
                    const AppContext &ctx, const CLIOptions &opts) {

    if (!app.got_subcommand(h.root)) return;

    buildWeeklyReport(ctx, opts.reportOpts);

}
