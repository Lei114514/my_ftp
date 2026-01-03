#include <stdexcept>
#include <iostream>

#include "FileSystem.h"

FileSystem::FileSystem(std::string rootPath)
{
    std::error_code ec;
    fs::path abs_root=fs::absolute(rootPath,ec);
    
    if(ec)
    {
        throw std::runtime_error("Invalid root path format");
    }

    if(!fs::exists(abs_root))
    {
        throw std::runtime_error("Root path does not exists: "+abs_root.string());
    }
    if(!fs::is_directory(abs_root))
    {
        throw std::runtime_error("Root path is not a directory: "+abs_root.string());
    }

    rootPath_=fs::canonical(abs_root,ec);
    currentPath_=".";

    //TODO 實現日志而不是單純的輸出
    std::cout<<"[FileSystem] Initialzed, rootPath:"<<rootPath_<<std::endl;
}

std::string FileSystem::getCurrentPath() const
{
    std::string path=currentPath_.generic_string();
    if(path.empty()||path==".")
    {
        return "/";
    }
    if(path.front()!='/')
    {
        return "/"+path;
    }
    return path;
}

std::string FileSystem::getRealFilePath(std::string fileName) const
{
    fs::path workPath;
    if(!fileName.empty()&&fileName[0]=='/')
    {
        workPath = rootPath_ / fileName.substr(1);
    }
    else 
    {
        workPath= rootPath_ / currentPath_ / fileName;
    }

    std::error_code ec;
    fs::path realPath = fs::weakly_canonical(workPath,ec);
    if(ec)
    {
        return "";
    }

    if(!isSafePath(realPath))
    {
        return "";
    }
    return realPath.string();
}

std::vector<std::string> FileSystem::list_files()
{
    std::vector<std::string> files;
    std::error_code ec;

    fs::path realPath = rootPath_ / currentPath_;
    if(!fs::exists(realPath,ec) || !fs::is_directory(realPath,ec) )
    {
        std::cerr<<"[FileSystem] List failed: "<<ec.message()<<std::endl;
        return files;
    }

    fs::directory_iterator dirIter(realPath,ec);
    if(ec)
    {
        std::cerr<<"[FileSystem] Iterator failed: "<<ec.message()<<std::endl;
        return files;
    }

    for(const auto& entry:dirIter)
    {
        std::error_code fileEc;
        std::string fileName=entry.path().filename().string();

        if(entry.is_regular_file(fileEc))
        {
            uintmax_t size=entry.file_size(fileEc);
            files.push_back(fileName+"\t("+std::to_string(size)+" bytes)");
        }
        else 
        {
            files.push_back(fileName+"\t<DIR>");
        }
    }
    return files;
}

bool FileSystem::change_dir(std::string path)
{
    if(path.empty()) return false;

    fs::path workPath;
    if(path[0]=='/')
    {
        workPath = rootPath_ / path; 
    }
    else 
    {
        workPath = rootPath_ / currentPath_ / path;
    }

    std::error_code ec;
    fs::path realPath = fs::canonical(workPath,ec);
    if(ec)
    {
        return false;
    }

    if(!isSafePath(realPath)) 
    {
        return false;
    }
    if(!fs::is_directory(realPath,ec))
    {
        return false;
    }
  
    currentPath_ = fs::relative(realPath,rootPath_,ec);
    return true;
}

bool FileSystem::isSafePath(const fs::path& target) const
{
    auto [rootIt,targetIt]=std::mismatch(rootPath_.begin(),rootPath_.end(),target.begin());
    if(rootIt!=rootPath_.end())
    {
        return false;
    }
    return true;
}