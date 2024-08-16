#include "createc.h"
#include <iostream>
#include <qt5/QtCore/QString>
#include <filesystem>

namespace fs = std::filesystem;

std::string VERSION = "v2.3_beta-1";

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        std::cerr << "No argument provided" << std::endl;
        return -1;
    }

    QString arg(argv[1]);

    // if the user is checking the version
    if (arg == "-v" || arg == "-V" || arg == "--version" || arg == "-version" || arg == "--v")
    {
        std::cout << "CreateC " << VERSION << std::endl;
        return 0;
    }

    // if the user is trying to build a project

    if (arg == "-b")
    {
        if (argc == 2)
        {
            std::cerr << "No input file is provided.\n";
            return -1;
        }
        auto buildFile = argv[2];

        if (buildFile == ".." || buildFile == "." || fs::is_directory(buildFile))
        {
            return build(QString("%1/create.json").arg(buildFile).toStdString().c_str());
        }

        return build(buildFile);
    }

    if (arg == "-i")
    {
        if (!fs::exists("./createc_cache.json"))
        {
            logError("Could not install executable onto the system. No cache file is found.");
            return -1;
        }
        json cacheFile = loadCacheFile();
        std::system(QString("sudo cp ./%1 /usr/bin").arg(QString::fromStdString(cacheFile["executable"])).toStdString().c_str());
    }

    if (arg == "-u")
    {
        if (!fs::exists("./createc_cache.json"))
        {
            logError("Could not install executable onto the system. No cache file is found.");
            return -1;
        }
        json cacheFile = loadCacheFile();
        std::system(QString("sudo rm -rf /usr/bin/%1").arg(QString::fromStdString(cacheFile["executable"])).toStdString().c_str());
    }

    return 0;
}
