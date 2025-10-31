#ifndef ABSTRACT_NETWORK_SERVICE_H
#define ABSTRACT_NETWORK_SERVICE_H

#include "AbstractClientService.h"

#include <memory>

class AbstractNetworkService
{
protected:
    static const int bufferSize_ = 1024;
    std::string address_;
    unsigned short port_;

public:
    AbstractNetworkService(const std::string &address, unsigned short p) : address_(std::move(address)), port_(p) {}
    virtual ~AbstractNetworkService() = default;
    virtual void bindAndListen() = 0;
    virtual std::unique_ptr<AbstractClientService> accept() = 0;
    virtual void close() = 0;
};

#endif // ABSTRACT_NETWORK_SERVICE_H