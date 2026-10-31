#pragma once
#include <iostream>
#include <vector>
#include "InetAddr.hpp"
#include "mylog.hpp"
#include "Cond && Mutex.hpp"
using namespace CondAndMutexModule;
using namespace LogModule;
using namespace std;

class Router
{
public:

    bool isExist(InetAddr& user)
    {
        for (auto& u : _userlist)
        {
            if (u == user)
                return true;
        }
        return false;
    }

    void AddUser(InetAddr& user)
    {
        LOG(Level::Info) << "加入一个新用户:" << user.Username();
        _userlist.push_back(user);
    }

    void DeleteUser(InetAddr& user)
    {
        LOG(Level::Info) << user.Username() << "用户退出";
        for (auto it = _userlist.begin(); it < _userlist.end(); it++)
        {
            if (*it == user)
                _userlist.erase(it);
            break;
        }
    }
    
    Router()
    {
    }

    void MessageRouter(int sockfd, const string& message, InetAddr& user)
    {
        LockGraud lockgraud(_mutex);
        if (!isExist(user))
        {
            AddUser(user);
        }

        string send_message = user.Username() + '#' + message;

        for (auto& u : _userlist)
        {
            sendto(sockfd, send_message.c_str(), send_message.size(), 0, (struct sockaddr*)&u.Sockaddr(), sizeof(u.Sockaddr()));
        }

        if (message == "QUIT")
        {
            DeleteUser(user);
        }
    }
private:
    vector<InetAddr> _userlist;
    Mutex _mutex;
};