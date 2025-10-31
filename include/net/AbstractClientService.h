#ifndef ABSTRACT_CLIENT_SERVICE_H
#define ABSTRACT_CLIENT_SERVICE_H

#include <string>

class AbstractClientService
{
public:
    virtual ~AbstractClientService() = default;
    virtual std::string read() = 0;
    virtual void write(std::string_view message) = 0;
    virtual void close() = 0;
};

#endif // ABSTRACT_CLIENT_SERVICE_H