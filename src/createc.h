#ifndef CREATEC_H
#define CREATEC_H
#include <qt5/QtCore/QString>
#include <json.hpp>
#include <iostream>

using json = nlohmann::json;

void logInfo(std::string text);
void logWarning(std::string text);
void logError(std::string text);
bool isPackageInstalled(std::string package);
void installPackage(std::string package);
void askToInstallPackage(std::string package);

void createCacheFile(QString projectName, QString executable);
json loadCacheFile();
int build(QString fileName);

#endif