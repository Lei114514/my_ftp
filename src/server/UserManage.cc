#include "UserManage.h"

bool UserManage::exist(const std::string& user)
{
    return userDb_.find(user) != userDb_.end();
}

bool UserManage::verify(const std::string& user,const std::string& password)
{
    auto it = userDb_.find(user);
    if(it!=userDb_.end()&&it->second==password)
    {
        return true;
    }
    return false;
}