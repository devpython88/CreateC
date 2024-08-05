#include <json.hpp>
#include <qt5/QtCore/QString>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;
using json = nlohmann::json;
const std::string VERSION = "1.0";

/****************************/
/****************************/

void logInfo(std::string text){
    std::cout << "[INFO] " << text << std::endl;
}

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

    if (!j.contains("sources")){
        std::cerr << "Source files not specified.\n";
        return -1;
    }

    // Optional
    int optimizationLevel = 0;
    json libraries = json::array();
    std::string cppStandard = "11";
    json includeDirs = json::array();
    json libDirs = json::array();

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

        if (standard != "11" || standard != "14" || standard != "17" || standard != "20" || standard != "23" || standard != "98"){
            std::cerr << "C++ standard is invalid!\n";
            return -1;
        }

        cppStandard = standard;
        logInfo("Set c++ standard to " + cppStandard);
    }

    // Make and run the command
    QString command("g++ -o ");
    command.append(qProjectName);
    command.append(" ");

    for (const auto& source : j["sources"]){
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

    return 0;
}
