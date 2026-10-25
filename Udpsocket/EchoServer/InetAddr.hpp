#pragma once
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace std;

class InetAddr
{
public:
    InetAddr(struct sockaddr_in& peer):_peer(peer)
    {
        _ip = inet_ntoa(_peer.sin_addr);
        _port = ntohs(_peer.sin_port);
    }

    uint16_t Port()
    {
        return _port;
    }

    string IP()
    {
        return _ip;
    }

private:
    struct sockaddr_in _peer;
    string _ip;
    uint16_t _port;
};