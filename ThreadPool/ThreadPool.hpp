#pragma once
#include "Cond && Mutex.hpp"
#include "mylog.hpp"
#include "thread.hpp"
#include <iostream>
#include <queue>

using namespace CondAndMutexModule;
using namespace LogModule;
using namespace PthreadMod;
using namespace std;

namespace ThreadPoolModule
{
    const int defaultnum = 5;
    template<class T>
    class ThreadPool
    {
    private:
        void WakeUpAllThread()
        {
            LOG(Level::Info) << "唤醒所有的休眠线程";
            _cond.Broadcast();
        }

        void WakeOneThread()
        {
            _cond.Signal();
            LOG(Level::Debug) << "唤醒一个休眠线程";
        }
        
    public:
        ThreadPool(const int num = defaultnum): _isrunning(false), _sleepnums(0)
        {
            LockGraud lockgraud(_mutex);
            for (int i = 0; i < num; i++)
            {
                _thread.emplace_back(
                    [this](){
                        Execute();
                    }
                );
            }
        }

        void Start()
        {
            if (_isrunning)
                return;
            _isrunning = true;
            for (auto& thread : _thread)
            {
                thread.Start();
                LOG(Level::Info) << "启动线程：" << thread.GetName() << " 成功";
            }
        }

        void Stop()
        {
            if (!_isrunning)
                return;
            _isrunning = false;
            WakeUpAllThread();
        }

        void Join()
        {
            for (auto& thread : _thread)
            {
                string name = thread.GetName();
                thread.Join();
                LOG(Level::Info) << "join pthread success:" << name;
            }
        }

        void Execute()
        {
            char name[128];
            pthread_getname_np(pthread_self(), name, sizeof(name));
            while (true)
            {
                T t;
                {
                    LockGraud lockgraud(_mutex);
                    while (_task_queue.empty() && _isrunning)
                    {
                        _sleepnums++;
                        _cond.Wait(_mutex);
                        _sleepnums--;
                    }

                    if (!_isrunning && _task_queue.empty())
                    {
                        LOG(Level::Debug) << name << "线程退出";
                        break;
                    }
                    t = _task_queue.front();
                    _task_queue.pop();
                }
                t();
            }
        }

        void Equeue(const T&& in)
        {
            LockGraud lockgraud(_mutex);
            _task_queue.push(in);
            if (_thread.size() == _sleepnums)
                WakeOneThread();
        }

    private:
        vector<Thread> _thread;
        queue<T> _task_queue;
        bool _isrunning;
        int _sleepnums;
        Mutex _mutex;
        Cond _cond;
    };
}
