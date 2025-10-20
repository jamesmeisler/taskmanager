//
// Created by james on 10/19/25.
//

#ifndef LEARNFTXUI_CONFIG_H
#define LEARNFTXUI_CONFIG_H

#define APP_NAME "LearnFtxUI"
#include <filesystem>

namespace fs = std::filesystem;

class Config {
private:
    fs::path dataPath;

    static fs::path xdgDataPath();

public:
    Config();
    fs::path const getDataPath();
    static fs::path ensureAppDataPath();
};


#endif //LEARNFTXUI_CONFIG_H