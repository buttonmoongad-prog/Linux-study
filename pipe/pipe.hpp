#pragma once
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include"TaskManage.hpp"
using namespace std;

class Channel {
public:
    Channel(int fd, int id): _fd(fd), _id(id)
    {
        _name = "channel-" + to_string(_fd) + to_string(_id);
    }

    void Send(int code)
    {
        int n = write(_fd, &code, sizeof(code));
    }

    void Close()
    {
        close(_fd);
    }

    void Wait()
    {
        cout << "子进程退出；" << _id << endl;
        waitpid(_id, nullptr, 0);
        sleep(1);
    }

    ~Channel(){}

private:
    int _fd;
    pid_t _id;
    string _name;
};

class ChannelManage
{
public:
    ChannelManage():_next(0)
    {}
    
    void Insert(int wfd, pid_t id)
    {
        _channels.emplace_back(wfd, id);
    }
    Channel& Select()
    {
        auto& c = _channels[_next++];
        _next %= _channels.size();
        return c;   
    }

    void StopSubChannel()
    {
        for (auto& ch : _channels)
        {
            ch.Close();
        }
    }

    void WaitSubChannel()
    {
        for (auto& ch : _channels)
        {
            ch.Wait();
        }
    }

    ~ChannelManage(){}


private:
    vector<Channel> _channels;
    int _next;
};

class ProcessPipe
{
public:
    ProcessPipe(int nums):_nums(nums)
    {
        _tk.Register(Printlog);
        _tk.Register(Download);
        _tk.Register(Update);
    }
    ~ProcessPipe(){}

    void Work(int rfd)
    {
        while (true)
        {
            int task_code = 0;
            ssize_t n = read(rfd, &task_code, sizeof(task_code));
            if (n > 0)
            {
                if (n != sizeof(task_code))
                {
                    continue;
                }
                _tk.Execute(task_code);
            }
            else if (n == 0)
            {
                // cout << "子进程退出" << endl;
                break;
            }
            else
            {
                cerr << "读取错误" << endl;
                break;
            }
        }
    }

    bool Create()
    {
        for (int i = 0; i < _nums; i++)
        {
            int fds[2];
            int n = pipe(fds);
            if (n < 0) return false;
            pid_t id = fork();
            if (id < 0)
            {
                cerr << "fork error" << endl;
                return false;
            }
            else if (id == 0)
            {
                close(fds[1]);
                Work(fds[0]);
                close(fds[0]);
                exit(0);
            }
            else
            {
                close(fds[0]);
                _cm.Insert(fds[1], id);
            }
        }
        return true;
    }

    void Run()
    {
        int taskcode = _tk.Code();
        auto& c = _cm.Select();
        c.Send(taskcode);
    }

    void Quit()
    {
        _cm.StopSubChannel();
        _cm.WaitSubChannel();
    }

private:
    int _nums;
    ChannelManage _cm;
    TaskManage _tk;
};