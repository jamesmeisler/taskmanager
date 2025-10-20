//
// Created by james on 10/19/25.
//

#include "config.h"

fs::path Config::xdgDataPath() {
    if (const char* xdg = std::getenv("XDG_DATA_PATH")) {
        fs::path xdgPath = xdg;
        if (xdgPath.is_absolute()) return xdgPath;
    }

    if (const char* home = std::getenv("HOME")) {
        return fs::path(home) / ".local" / "share";
    }

    throw std::runtime_error("XDG_DATA_PATH not set");
}

Config::Config() : dataPath(xdgDataPath()) {}

fs::path const Config::getDataPath() {
    return dataPath / APP_NAME;
}

fs::path Config::ensureAppDataPath() {
    fs::path dir = xdgDataPath() / APP_NAME;
    std::error_code ec;
    fs::create_directories(dir, ec);
    if (ec) throw std::runtime_error("Could not create app data directory: " +  ec.message());

    //Tighten permissions
    std::error_code ec2;
    fs::permissions(dir,
        fs::perms::owner_all,
        fs::perm_options::add,
        ec2);

    return dir;
}
