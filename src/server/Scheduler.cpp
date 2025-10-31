#include "server/Scheduler.h"

std::pair<std::string_view, unsigned short> Scheduler::getNextServer()
{
    if (serverQueue_.empty())
    {
        throw std::runtime_error("No backend servers available");
    }

    std::pair<std::string_view, unsigned short> server = serverQueue_.front();
    serverQueue_.pop();
    serverQueue_.push(server);
    return server;
}