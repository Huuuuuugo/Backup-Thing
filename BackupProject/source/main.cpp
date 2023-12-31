#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <mz_zip.h>
#include <zip.h>

#include "zipFolder.h"
#include "timeString.h"

int createBackup(std::string, std::string);
std::string fixPath(char const* inArg);

int main(int argc, char const* argv[]) {
    std::string timeString = currentDateTime();
    std::string zipFilePath = fixPath(argv[2]) + '/' + timeString + ".zip";
    std::string sourceFolderPath = fixPath(argv[1]);

    if (createBackup(zipFilePath, sourceFolderPath)) return 1;

    return 0;
}


int createBackup(std::string zipFilePath, std::string sourceFolderPath) {
    zipFile zip = zipOpen(zipFilePath.c_str(), APPEND_STATUS_CREATE);  //creates a zip file to be written to
    if (zip == nullptr) {
        std::cout << "Failed to create the zip file" << std::endl;
        return 1;
    }

    int result = zipFolder(zip, sourceFolderPath, ""); //calls the function in "zipFolder.cpp" that adds the contens of the folder to the zip
    if (result != ZIP_OK) {
        std::cout << "Failed to add the folder the zip" << std::endl;
        zipClose(zip, nullptr);
        return 1;
    }

    result = zipClose(zip, nullptr); //closes the zip file that was being used
    if (result != ZIP_OK) {
        std::cout << "Failed to save the folder the zip" << std::endl;
    }

    std::cout << "Zip file created successfuly: " << zipFilePath << std::endl;

    return 0;
}


std::string fixPath (char const* inArg) {
    std::string str = inArg;
    std::ostringstream strm;
    for (char c : str) {
        if (c == '\\') {
            strm << '/';
        }
        else {
            strm << c;
        }
    }
    str = strm.str();

    return str;
}