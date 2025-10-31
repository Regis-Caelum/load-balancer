#include "net/WindowsClientService.h"

#include <iostream>
#include <string>

std::string WindowsClientService::read()
{
    char buffer[bufferSize_];
    int bytesReceived = recv(socket_, buffer, bufferSize_ - 1, 0);
    if (bytesReceived == SOCKET_ERROR)
    {
        throw std::runtime_error("Receive failed: " + std::to_string(WSAGetLastError()));
    }
    buffer[bytesReceived] = '\0';
    return std::string(buffer);
}

void WindowsClientService::write(std::string_view message)
{
    const char *buffer = message.data();
    int totalSent = 0;
    int remaining = static_cast<int>(message.size());

    while (remaining > 0)
    {
        int sent = ::send(socket_, buffer + totalSent, remaining, 0);
        if (sent == SOCKET_ERROR)
        {
            throw std::runtime_error("Send failed: " + std::to_string(WSAGetLastError()));
        }
        totalSent += sent;
        remaining -= sent;
    }
}

void WindowsClientService::close()
{
    closesocket(socket_);
    std::cout << "Client socket closed.\n";
}