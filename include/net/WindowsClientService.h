#ifndef WINDOWS_CLIENT_SERVICE_H
#define WINDOWS_CLIENT_SERVICE_H

#include "AbstractClientService.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class WindowsClientService : public AbstractClientService
{
private:
    SOCKET socket_;
    static const int bufferSize_ = 1024;

public:
    explicit inline WindowsClientService(SOCKET s) : socket_(s) {}
    inline ~WindowsClientService()
    {
        close();
    }
    std::string read() override;
    void write(std::string_view message) override;
    void close() override;
};

#endif // WINDOWS_CLIENT_SERVICE_H