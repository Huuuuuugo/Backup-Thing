//all the code here was taken from https://youtu.be/7bbu3EYKf_8?si=7ovLzslznmEmrPTv
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

#include <mz_zip.h>
#include <zip.h>


int fileToZip(zipFile zf, const std::string& filePath, const std::string& nameOfFileInZip)
{
    std::ifstream file(filePath, std::ios::binary); //opens the file on the especified directory as a binary stream
    if (!file)
    {
        std::cout << "Failed to open file" << std::endl;
        return ZIP_INTERNALERROR;
    }

    file.seekg(0, std::ios::end); //points to the end of the file
    std::streamsize fileSize = file.tellg(); //saves the previosly pointed data into "fileSize"
    file.seekg(0, std::ios::beg); //points back to the beginning of the file

    int result = zipOpenNewFileInZip(zf, nameOfFileInZip.c_str(), nullptr, nullptr, 0, nullptr, 0, nullptr, Z_DEFLATED, 9); //creates a blank file in the zip
    if (result != ZIP_OK)
    {
        std::cout << "Failed to create a new file in the zip" << std::endl;
        file.close();
        return result;
    }

    std::vector<char> fileBuffer(fileSize); //cretes an buffer with the exact size of the file
    if (!file.read(fileBuffer.data(), fileSize)) //write the contents of the stream "file" into the vector "fileBuffer"
    {
        std::cout << "Failed to read the source file: " << filePath << std::endl;
        file.close();
        return ZIP_INTERNALERROR;
    }

    result = zipWriteInFileInZip(zf, fileBuffer.data(), static_cast<unsigned int>(fileSize)); //writes the contents of "fileBuffer" into the blank file previously created into the zip file
    if (result != ZIP_OK)
    {
        std::cout << "Failed to write file content into the zip" << std::endl;
        file.close();
        return result;
    }

    file.close();
    result = zipCloseFileInZip(zf); //closes the zip file
    if (result != ZIP_OK)
    {
        std::cout << "Failed to close file in the zip" << std::endl;
        return result;
    }

    return ZIP_OK;
}



int zipFolder(zipFile zf, const std::string& folderPath, const std::string& folderNameInZip)
{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath))
    {
        if (!entry.is_regular_file()) continue;
        std::string filePath = entry.path().string();
        std::string relativePath = entry.path().string().substr(folderPath.size() + 1);
        std::string fileNameInZip = folderNameInZip + relativePath;
        int result = fileToZip(zf, filePath, fileNameInZip);
        if (result != ZIP_OK)
        {
            std::cout << "Failed add file to the zip: " << filePath << std::endl;
            return result;
        }
    }
    return ZIP_OK;
}
