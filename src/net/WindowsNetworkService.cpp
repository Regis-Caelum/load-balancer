#include "net/WindowsNetworkService.h"
#include "net/WindowsClientService.h"

#include <iostream>
#include <string>

void WindowsNetworkService::bindAndListen()
{
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;

    if (InetPtonA(AF_INET, address_.c_str(), &(serverAddr.sin_addr)) != 1)
    {
        throw std::runtime_error("Invalid IP address: " + address_);
    }
    serverAddr.sin_port = htons(port_);

    if (bind(socket_, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
    {
        throw std::runtime_error("Bind failed: " + std::to_string(WSAGetLastError()));
    }

    if (listen(socket_, SOMAXCONN) == SOCKET_ERROR)
    {
        throw std::runtime_error("Listen failed: " + std::to_string(WSAGetLastError()));
    }

    std::cout << "Server listening on port " << port_ << std::endl;
}
std::unique_ptr<AbstractClientService> WindowsNetworkService::accept()
{
    sockaddr_in clientAddr{};
    int clientSize = sizeof(clientAddr);
    SOCKET clientSocket = ::accept(socket_, reinterpret_cast<sockaddr *>(&clientAddr), &clientSize);
    if (clientSocket == INVALID_SOCKET)
    {
        throw std::runtime_error("Accept failed: " + std::to_string(WSAGetLastError()));
    }
    std::cout << "Using Windows network service." << std::endl;
    std::cout << "Client connected!\n";
    return std::make_unique<WindowsClientService>(clientSocket);
}

void WindowsNetworkService::close()
{
    closesocket(socket_);
    std::cout << "Socket closed.\n";
}