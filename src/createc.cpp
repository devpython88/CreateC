#include <json.hpp>
#include <qt5/QtCore/QString>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;
using json = nlohmann::json;
const std::string VERSION = "v1.61";

void logInfo(std::string text){
    std::cout << "[INFO] " << text << std::endl;
}

void logError(std::string text){
    std::cout << "[ERROR] " << text << std::endl;
}

void createCacheFile(QString projectName, QString executable){
    json cacheData;

    cacheData["project"] = projectName.toStdString();
    cacheData["executable"] = executable.toStdString();

    std::ofstream file("createc_cache.json");
    if (file.is_open()){
        file << cacheData.dump(4);
        file.close();
        return;
    } else {
        logError("Could not create cache file.");
    }
}

json loadCacheFile(){
    json cacheFile;

    std::ifstream file("createc_cache.json");
    if (file.is_open()){
        file >> cacheFile;
        file.close();
        return cacheFile;
    } else {
        logError("Could not create cache file.");
        return json();
    }
}

/****************************/
/****************************/

int build(QString fileName){
    std::ifstream file(fileName.toStdString());

    if (!file.is_open()){
        std::cerr << "Could not load file!\n";
        return -1;
    }

    json j;
    file >> j;
    
    if (!j.contains("project")){
        std::cerr << "Project name not specified.\n";
        return -1;
    }

    std::string projectName = j["project"];
    QString qProjectName(QString::fromStdString(projectName));
    QString executableName(qProjectName);
    json cacheFile;
    json previousCacheFile;

    if (j.contains("exe")){
        executableName = QString::fromStdString(j["exe"]);
    }

    if (!fs::exists("./createc_cache.json")){
        createCacheFile(qProjectName, executableName);
    } else {
        previousCacheFile = loadCacheFile();
        std::system("rm -rf ./createc_cache.json");
        createCacheFile(qProjectName, executableName);
        cacheFile = loadCacheFile();
    }

    if (!j.contains("sources")){
        std::cerr << "Source files not specified.\n";
        return -1;
    }

    // Optional
    int optimizationLevel = 0;
    json libraries = json::array();
    std::string cppStandard = "11";
    std::string cStandard = "11";
    json includeDirs = json::array();
    json libDirs = json::array();

    auto sourcefiles = j["sources"];

    if (j.contains("cleanBuild")){
        bool doACleanBuild = j["cleanBuild"];

        if (doACleanBuild){
            if (!previousCacheFile.contains("executable")){
                logError("Could not do a cleanBuild, No executable found in cache file (cache file might not exist. Try removing 'cleanBuild' temporarily.)");
                return -1;
            }

            std::system(QString("rm -rf ./%1").arg(QString::fromStdString(previousCacheFile["executable"])).toStdString().c_str());
        }
    }

    if (j.contains("includes")){
        includeDirs = j["includes"];
    }
    
    if (j.contains("libdirs")){
        libDirs = j["libdirs"];
    }
    
    if (j.contains("optlvl")){
        optimizationLevel = j["optlvl"];
        if (optimizationLevel < 1 || optimizationLevel > 3){
            std::cerr << "Invalid optimization level!\n";
            return -1;
        }

        logInfo(QString("Set optimization level to %1.").arg(optimizationLevel).toStdString());
    }

    if (j.contains("libraries")){
        libraries = j["libraries"];
        for (const auto& lib : libraries){
            QString text = QString("Added library `%1` to executable.").arg(QString::fromStdString(lib));
            logInfo(text.toStdString());
        }
    }

    if (j.contains("cppstd")){
        std::string standard = j["cppstd"];

        if (standard != "11" && standard != "14" && standard != "17" && standard != "20" && standard != "23" && standard != "98"){
            std::cerr << "C++ standard is invalid!\n";
            return -1;
        }

        cppStandard = standard;
        logInfo("Set c++ standard to " + cppStandard);
    }

    if (j.contains("cstd")){
        std::string standard = j["cstd"];

        if (standard != "11" && standard != "14" && standard != "17" && standard != "20" && standard != "23"){
            std::cerr << "Clang standard is invalid!\n";
            return -1;
        }

        cStandard = standard;
        logInfo("Set clang standard to " + cStandard);
    }

    /* END */

    // Make and run the command
    QString command("g++ -o ");
    command.append(executableName);
    command.append(" ");

    for (const auto& source : sourcefiles){
        command.append(QString::fromStdString(source));
        command.append(" ");
    }

    if (optimizationLevel > 0){
        command.append("-O");
        command.append(QString::fromStdString(std::to_string(optimizationLevel)));
        command.append(" ");
    }

    if (!libraries.empty()){
        for (const auto& lib : libraries){
            if (lib == "gtkmm"){
                QString text("`pkg-config --cflags --libs gtkmm-3.0`");
                text.append(" ");
                command.append(text);
                continue;
            }

            else if (lib == "gtk3"){
                QString text("`pkg-config --cflags --libs gtk+-3.0` ");
                command.append(text);
                continue;
            }

            else if (lib == "gtk4"){
                QString text("`pkg-config --cflags --libs gtk+-4.0` ");
                command.append(text);
                continue;
            }

            else if (lib == "qt5widgets"){
                QString text("`pkg-config --cflags --libs Qt5Widgets` -fPIC ");
                command.append(text);
                continue;
            }
            
            else if (lib == "qt5core"){
                QString text("`pkg-config --cflags --libs Qt5Core` -fPIC ");
                command.append(text);
                continue;
            }
            
            else if (lib == "qt5gui"){
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

    if (!includeDirs.empty()){
        for (const auto& dir : includeDirs){
            command.append(QString("-I%1 ").arg(QString::fromStdString(dir)));
        }
    }

    if (!libDirs.empty()){
        for (const auto& dir : libDirs){
            command.append(QString("-L%1 ").arg(QString::fromStdString(dir)));
        }
    }

    // execute
    int result = std::system(command.toStdString().c_str());
    if (result != 0){
        std::cout << "Building completed with errors.\n";
        return -1;
    }

    std::cout << "Building completed.\n";
    return 0;
}

/****************************/
/****************************/

int main(int argc, char const *argv[])
{
    if (argc == 1){
        std::cerr << "No argument provided" << std::endl;
        return -1;
    }

    QString arg(argv[1]);

    // if the user is checking the version
    if (arg == "-v" || arg == "-V" || arg == "--version" || arg == "-version" || arg == "--v"){
        std::cout << "CreateC " << VERSION << std::endl;
        return 0;
    }

    // if the user is trying to build a project

    if (arg == "-b"){

        if (argc == 2){
            std::cerr << "No input file is provided.\n";
            return -1;
        }

        return build(argv[2]);
    }

    if (arg == "-i"){
        if (!fs::exists("./createc_cache.json")){
            logError("Could not install executable onto the system. No cache file is found.");
            return -1;
        }
        json cacheFile = loadCacheFile();
        std::system(QString("sudo cp ./%1 /usr/bin").arg(QString::fromStdString(cacheFile["executable"])).toStdString().c_str());
    }

    if (arg == "-u"){
        if (!fs::exists("./createc_cache.json")){
            logError("Could not install executable onto the system. No cache file is found.");
            return -1;
        }
        json cacheFile = loadCacheFile();
        std::system(QString("sudo rm -rf /usr/bin/%1").arg(QString::fromStdString(cacheFile["executable"])).toStdString().c_str());
    }

    return 0;
}
