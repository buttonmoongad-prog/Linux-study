#pragma once
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "mylog.hpp"
#include <string.h>
using namespace LogModule;
using namespace std;

class EchoServer
{
public:
    EchoServer(uint16_t& port)
        :_port(port),
        _sockfd(-1),
        _isrunning(false)
    {
    }

    void Init()
    {
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            LOG(Level::Fatal) << "socket fail";
            exit(1);
        }
        else
        {
            LOG(Level::Info) << "socket success";
        }
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = INADDR_ANY;

        int n = bind(_sockfd, (struct sockaddr*)&local, sizeof(local));
        if (n < 0)
        {
            LOG(Level::Fatal) << "bind fail";
            exit(1);
        }
        else
        {
            LOG(Level::Info) << "bind success";
        }
    }

    void Start()
    {
        if(!_isrunning)
            _isrunning = true;
        while (_isrunning)
        {
            char buffer[1024];
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int n = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&peer, &len);
            if (n > 0)
            {
                buffer[n] = 0;
                LOG(Level::Info) << buffer;
                string echo_string = "client say#";
                echo_string += buffer;
                int s = sendto(_sockfd, echo_string.c_str(), echo_string.size(), 0, (struct sockaddr*)&peer, len);
            }
        }
    }

private:
    int _sockfd;
    uint16_t _port;
    bool _isrunning;
};