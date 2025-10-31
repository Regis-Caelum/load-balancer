#ifndef WINDOWS_NETWORK_SERVICE_H
#define WINDOWS_NETWORK_SERVICE_H

#include "AbstractNetworkService.h"
#include "AbstractClientService.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

class WindowsNetworkService : public AbstractNetworkService
{
private:
    SOCKET socket_;

public:
    inline WindowsNetworkService(std::string address, unsigned short p = 8080) : AbstractNetworkService(address, p)
    {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            std::cerr << "WSAStartup failed\n";
            throw std::runtime_error("WSAStartup failed");
        }

        address_ = std::move(address);
        port_ = p;

        socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socket_ == INVALID_SOCKET)
        {
            std::cerr << "Socket creation failed\n";
            WSACleanup();
            throw std::runtime_error("Socket creation failed");
        }
    };

    ~WindowsNetworkService()
    {
        closesocket(socket_);
        WSACleanup();
    }

    void bindAndListen() override;
    std::unique_ptr<AbstractClientService> accept() override;
    void close() override;
};

#endif // WINDOWS_NETWORK_SERVICE_H