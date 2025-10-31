#include "server/Server.h"
#include "net/WindowsClientService.h"

#include <iostream>

void Server::listen()
{
    networkService_->bindAndListen();
    auto clientService = networkService_->accept();

    std::string msg = clientService->read();
    std::cout << "Message from client: " << msg << std::endl;

    clientService->write("Hello from load balancer!");
    clientService->write("Forwarding to backend server...");
    auto forwardService = forward();

    forwardService->write(msg);
    forwardService->write("EOF");

    std::string response = forwardService->read();
    forwardService->close();

    std::cout << "Response from backend server: " << response << std::endl;
    clientService->write(response);

    clientService->close();
}

std::unique_ptr<AbstractClientService> Server::forward()
{
    SOCKET backendSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (backendSocket == INVALID_SOCKET)
    {
        throw std::runtime_error("Backend socket creation failed: " + std::to_string(WSAGetLastError()));
    }

    std::pair<std::string_view, unsigned short> backendServer = scheduler_->getNextServer();

    std::string_view backendAddress = backendServer.first;
    unsigned short backendPort = backendServer.second;

    sockaddr_in backendAddr{};
    backendAddr.sin_family = AF_INET;

    if (InetPtonA(AF_INET, backendAddress.data(), &(backendAddr.sin_addr)) != 1)
    {
        throw std::runtime_error("Invalid IP address: " + std::string(backendAddress));
    }
    backendAddr.sin_port = htons(backendPort);

    if (connect(backendSocket, reinterpret_cast<sockaddr *>(&backendAddr), sizeof(backendAddr)) == SOCKET_ERROR)
    {
        throw std::runtime_error("Backend connection failed: " + std::to_string(WSAGetLastError()));
    }
    std::cout << "Connected to backend server " << backendAddress << ":" << backendPort << std::endl;

    return std::make_unique<WindowsClientService>(backendSocket);
}