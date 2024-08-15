#include <json.hpp>
#include <qt5/QtCore/QString>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;
using json = nlohmann::json;
const std::string VERSION = "v2.13-2";

std::string infoColor = "\033[34m";
std::string errorColor = "\033[31m";

void logInfo(std::string text)
{
    std::cout << infoColor << "[INF] \033[0m" << text << std::endl;
}

void logError(std::string text)
{
    std::cout << errorColor << "[ERR] \033[0m" << text << std::endl;
}

void createCacheFile(QString projectName, QString executable)
{
    json cacheData;

    cacheData["project"] = projectName.toStdString();
    cacheData["executable"] = executable.toStdString();

    std::ofstream file("createc_cache.json");
    if (file.is_open())
    {
        file << cacheData.dump(4);
        file.close();
        return;
    }
    else
    {
        logError("Could not create cache file.");
    }
}

json loadCacheFile()
{
    json cacheFile;

    std::ifstream file("createc_cache.json");
    if (file.is_open())
    {
        file >> cacheFile;
        file.close();
        return cacheFile;
    }
    else
    {
        logError("Could not load cache file.");
        return json();
    }
}

void runScripts(auto scripts)
{
    for (const auto &script : scripts)
    {
        int res = std::system(QString("bash %1").arg(QString::fromStdString(script)).toStdString().c_str());
        if (res != 0)
        {
            logError(QString("Error in script: %1").arg(QString::fromStdString(script)).toStdString().c_str());
            std::exit(-1);
        }
    }
}

std::string getANSICode(std::string color)
{
    // forced to use if-else, switch doesnt work on strings
    if (color == "red")
        return "\033[31m";
    else if (color == "yellow")
        return "\033[33m";
    else if (color == "green")
        return "\033[32m";
    else if (color == "blue")
        return "\033[34m";

    return "!";
}

void changeColor(std::string color, std::string *dest)
{
    std::string clr;
    if ((clr = getANSICode(color)) != "!")
    {
        *dest = clr;
        return;
    }

    logError("Invalid color `" + color + "` in createc_colors.json");
    std::exit(-1);
}

void changeColors(json j)
{
    if (j.contains("infoColor"))
        changeColor(j["infoColor"], &infoColor);
    else if (j.contains("errorColor"))
        changeColor(j["errorColor"], &errorColor);
}

void readColorsFile(std::string pathToColorsFile)
{
    json j;
    std::ifstream file(pathToColorsFile);

    if (!file.is_open())
    {
        logError("Could not load color file.");
        std::exit(-1);
    }

    file >> j;

    changeColors(j);
}

/****************************/
/****************************/

int build(QString fileName, std::string pathToColorsFile)
{
    std::ifstream file(fileName.toStdString());

    if (!fs::exists(pathToColorsFile))
    {
        logError("color file does not exist: " + pathToColorsFile);
        return -1;
    }

    readColorsFile(pathToColorsFile);

    if (!file.is_open())
    {
        std::cerr << "Could not load file!\n";
        return -1;
    }

    json j;
    file >> j;

    if (!j.contains("project"))
    {
        std::cerr << "Project name not specified.\n";
        return -1;
    }

    std::string projectName = j["project"];
    QString qProjectName(QString::fromStdString(projectName));
    QString executableName(qProjectName);
    QString outputDirectory("./");
    json cacheFile;
    json previousCacheFile;

    if (j.contains("exe"))
    {
        executableName = QString::fromStdString(j["exe"]);
    }

    if (j.contains("outputDir"))
    {
        outputDirectory = QString::fromStdString(j["outputDir"]);
    }

    if (!fs::exists("./createc_cache.json"))
    {
        createCacheFile(qProjectName, QString("%1%2").arg(outputDirectory, executableName));
    }
    else
    {
        previousCacheFile = loadCacheFile();
        std::system("rm -rf ./createc_cache.json");
        createCacheFile(qProjectName, QString("%1%2").arg(outputDirectory, executableName));
        cacheFile = loadCacheFile();
    }

    if (!j.contains("sources"))
    {
        logError("Source files not specified.");
        return -1;
    }

    // Optional
    int optimizationLevel = 0;
    json libraries = json::array();
    int cppStandard = 11;

    json scriptsToRunAfter;
    json scriptsToRunBefore;

    auto sourcefiles = j["sources"];

    if (j.contains("cleanBuild"))
    {
        bool doACleanBuild = j["cleanBuild"];

        if (doACleanBuild)
        {
            if (!previousCacheFile.contains("executable"))
            {
                logError("Could not do a cleanBuild, No executable found in cache file (cache file might not exist. Try removing 'cleanBuild' temporarily.)");
                return -1;
            }

            std::system(QString("rm -rf ./%1").arg(QString::fromStdString(previousCacheFile["executable"])).toStdString().c_str());
        }
    }

    if (j.contains("optlvl"))
    {
        optimizationLevel = j["optlvl"];
        if (optimizationLevel < 1 || optimizationLevel > 3)
        {
            std::cerr << "Invalid optimization level!\n";
            return -1;
        }

        logInfo(QString("Set optimization level to %1.").arg(optimizationLevel).toStdString());
    }

    if (j.contains("libraries"))
    {
        libraries = j["libraries"];
        for (const auto &lib : libraries)
        {
            QString text = QString("Added library `%1` to executable.").arg(QString::fromStdString(lib));
            logInfo(text.toStdString());
        }
    }

    if (j.contains("cppstd"))
    {
        int standard = j["cppstd"];

        if (standard != 11 && standard != 14 && standard != 17 && standard != 20 && standard != 23)
        {
            logError("C standard is invalid!");
            return -1;
        }

        cppStandard = standard;
        logInfo("Set clang standard to " + std::to_string(cppStandard));
    }

    /* END */
    /* SCRIPTS */
    if (j.contains("scripts"))
    {
        auto scripts = j["scripts"];

        if (scripts.contains("after"))
        {
            scriptsToRunAfter = scripts["after"];
        }

        if (scripts.contains("before"))
        {
            scriptsToRunBefore = scripts["before"];
            runScripts(scriptsToRunBefore);
        }
    }

    // Make and run the command
    QString command("g++ -o ");
    command.append(QString("%1%2").arg(outputDirectory, executableName));
    command.append(QString::fromStdString(" -std=c++" + std::to_string(cppStandard) + " "));

    for (const auto &source : sourcefiles)
    {
        command.append(QString::fromStdString(source));
        command.append(" ");
    }

    if (j.contains("verbose"))
    {
        if (j["verbose"] == true)
        {
            command.append("-v ");
        }
    }

    if (optimizationLevel > 0)
    {
        command.append("-O");
        command.append(QString::fromStdString(std::to_string(optimizationLevel)));
        command.append(" ");
    }

    if (!libraries.empty())
    {
        for (const auto &lib : libraries)
        {
            if (lib == "gtkmm")
            {
                QString text("`pkg-config --cflags --libs gtkmm-3.0`");
                text.append(" ");
                command.append(text);
                continue;
            }

            else if (lib == "gtk3")
            {
                QString text("`pkg-config --cflags --libs gtk+-3.0` ");
                command.append(text);
                continue;
            }

            else if (lib == "gtk4")
            {
                QString text("`pkg-config --cflags --libs gtk+-4.0` ");
                command.append(text);
                continue;
            }

            else if (lib == "qt5widgets")
            {
                QString text("`pkg-config --cflags --libs Qt5Widgets` -fPIC ");
                command.append(text);
                continue;
            }

            else if (lib == "qt5core")
            {
                QString text("`pkg-config --cflags --libs Qt5Core` -fPIC ");
                command.append(text);
                continue;
            }

            else if (lib == "qt5gui")
            {
                QString text("`pkg-config --cflags --libs Qt5Gui` -fPIC ");
                command.append(text);
                continue;
            }

            QString text("-l");
            text.append(QString::fromStdString(lib));
            text.append(" ");
            command.append(text);
        }
    }

    std::cout << "\033[33mBuilding started.. [50%]\033[0m\n";

    // execute
    int result = std::system(command.toStdString().c_str());
    if (result != 0)
    {
        std::cout << errorColor + "Building completed with errors.\033[0m\n";
        return -1;
    }

    if (!scriptsToRunAfter.empty())
    {
        runScripts(scriptsToRunAfter);
    }

    std::cout << "\033[32mBuilding completed. [100%]\033[0m\n";
    return 0;
}

/****************************/
/****************************/

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        std::cerr << "No argument provided" << std::endl;
        return -1;
    }

    QString arg(argv[1]);

    std::string colorsFile = "./createc_colors.json";

    if (!fs::exists(colorsFile))
        colorsFile = "~/.local/createc_colors.json";
    else
        logError("No color file is found (searched in ./ and ~/.local/)");

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
            return build(QString("%1/create.json").arg(buildFile).toStdString().c_str(), colorsFile);
        }

        return build(buildFile, colorsFile);
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
