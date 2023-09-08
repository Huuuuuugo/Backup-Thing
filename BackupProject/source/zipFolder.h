#pragma once
#include <string>
#include <zip.h>

int fileToZip(zipFile zf, const std::string& filePath, const std::string& nameOfFileInZip);

int zipFolder(zipFile zf, const std::string& folderPath, const std::string& folderNameInZip);