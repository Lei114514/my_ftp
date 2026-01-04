#pragma once

#include <string>
#include <map>

class UserManage
{
public:
    UserManage()=default;
    ~UserManage()=default;

    bool exist(const std::string& user);
    bool verify(const std::string& user,const std::string& password);
private:    
    std::map<std::string,std::string> userDb_={
        {"admin","admin123"},
        {"user","user123"}
    };
};