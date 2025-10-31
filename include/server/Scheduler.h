#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <queue>
#include <string>
#include <iostream>

class Scheduler
{
private:
    std::queue<std::pair<std::string_view, unsigned short>> serverQueue_;

public:
    inline Scheduler()
    {
        serverQueue_.emplace("127.0.0.1", 5001);
    };

    ~Scheduler() = default;

    std::pair<std::string_view, unsigned short> getNextServer();
};
#endif // SCHEDULER_H