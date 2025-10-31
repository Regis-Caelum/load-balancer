#ifndef SERVER_H
#define SERVER_H

#include "net/WindowsNetworkService.h"
#include "Scheduler.h"

#include <memory>
#include <queue>

class Server
{
private:
    std::string address_;
    unsigned short port_;
    std::unique_ptr<Scheduler> scheduler_;
    std::unique_ptr<AbstractNetworkService> networkService_;

public:
    Server(std::string_view address, unsigned short port) : address_(address), port_(port)
    {
#ifdef _WIN32
        networkService_ = std::make_unique<WindowsNetworkService>(address_, port_);
#else
        // Placeholder for other OS network service implementations
#endif
        scheduler_ = std::make_unique<Scheduler>();
    }

    void listen();
    std::unique_ptr<AbstractClientService> forward();
};

#endif // SERVER_H