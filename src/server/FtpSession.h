#pragma once

#include <string>
#include <vector>
#include <memory>
#include <atomic>

class CommandInterpreter;
class FileSystem;
class DataConnection;

class FtpSession
{
public:
    explicit FtpSession(int socketFd);
    ~FtpSession();

    FtpSession(const FtpSession&) = delete;
    FtpSession& operator=(const FtpSession&) = delete;

    void start();
    void sendControl(const std::string& message);

    FileSystem* getFileSystem() const;
    DataConnection* getDataConnection() const;

    void setUserName(const std::string& name);
    std::string getUserName() const;
    
    void setLoggedIn(bool login);
    bool isLoggedIn() const;

    void shutdown();
private:
    bool processBuffer();
    int fd_;        
    std::atomic<bool> isRunning_; 
    
    std::string buffer_;

    std::unique_ptr<CommandInterpreter> interpreter_;
    std::unique_ptr<FileSystem> fileSystem_;
    std::unique_ptr<DataConnection> dataConnection_;

    // 4. 會話狀態數據
    std::string userName_;
    bool isLoggedIn_;
};