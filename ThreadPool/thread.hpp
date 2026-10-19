#pragma once
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <cstdio>
#include <cstring>
#include <functional>
using namespace std;

namespace PthreadMod
{
    using func_t = function<void()>;
    static int count = 1;
    class Thread
    {
    private:
        static void* Rountie(void* args)
        {
            Thread* self = static_cast<Thread*>(args);
            self->Running();
            if (self->_isdetach)
                self->Detach();
            self->_func();
            return nullptr;
        }

        void Running()
        {
            if (_isrunning)
                return;
            _isrunning = true;
        }

    public:
        Thread(func_t func)
            :_tid(0)
            ,_isdetach(false)
            ,_isrunning(false)
            ,_func(func)
        {
            _name = "thread-" + to_string(count++);
        }

        void Join()
        {
            if (_isdetach)
            {
                cout << "该线程被分离" << endl;
                return;
            }
            int n = pthread_join(_tid, nullptr);
            if (n == 0)
            {
            }
            else
            {
                cerr << "pthread join error" << endl;
            }
        }

        bool Start()
        {
            if (_isrunning)
                return false;
            int n = pthread_create(&_tid, nullptr, Rountie, this);
            return true;
        }

        string GetName()
        {
            return _name;
        }

        void Detach()
        {
            if(_isdetach)
                return;
            if(_isrunning)
            {
                pthread_detach(_tid);
            }
            _isdetach = true;
        }

        void Stop()
        {
            if (_isrunning)
            {
                int n = pthread_cancel(_tid);
                if (n == 0)
                {
                    _isrunning = false;
                    cout << "线程停止" << endl;
                }
                else
                {
                    cerr << "pthread cancel error" << endl;
                }
            }
        }

        ~Thread()
        {}
        
    private:
        pthread_t _tid;
        string _name;
        bool _isdetach;
        bool _isrunning;
        func_t _func;
    };
}