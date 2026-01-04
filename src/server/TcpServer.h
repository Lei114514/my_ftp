#pragma once

#include <string>

class TcpServer
{
public:
    TcpServer(std::string password);
    ~TcpServer();
    void start();

private:
    int listenFd_;
    int epollFd_;
};