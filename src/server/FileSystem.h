#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class FileSystem
{
public:
    FileSystem(std::string rootPath);
    std::string getCurrentPath() const;
    std::string getRealFilePath(std::string fileName) const;

    std::vector<std::string> list_files();
    bool change_dir(std::string path);

private:
    fs::path currentPath_;
    fs::path rootPath_;
    bool isSafePath(const fs::path& target) const;
};  