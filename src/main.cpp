#include "net/WindowsNetworkService.h"
#include "server/Server.h"

#include <iostream>

int main()
{
    Server server("127.0.0.1", 5000);
    server.listen();
    return 0;
}
