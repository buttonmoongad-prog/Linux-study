#pragma once
#include <iostream>
#include <pthread.h>

namespace CondAndMutexModule
{
    class Mutex
    {
    public:
        Mutex()
        {
            pthread_mutex_init(&_mutex, nullptr);
        }

        void Lock()
        {
            pthread_mutex_lock(&_mutex);
        }

        void Unlock()
        {
            pthread_mutex_unlock(&_mutex);
        }

        pthread_mutex_t* Get()
        {
            return &_mutex;
        }

        ~Mutex()
        {
            pthread_mutex_destroy(&_mutex);
        }
        
    private:
        pthread_mutex_t _mutex;
    };

    class LockGraud
    {
    public:
        LockGraud(Mutex& mutex):_mutex(mutex)
        {
            _mutex.Lock();
        }
        ~LockGraud()
        {
            _mutex.Unlock();
        }
    private:
        Mutex _mutex;
    };

    class Cond
    {
    public:
        Cond()
        {
            pthread_cond_init(&_cond, nullptr);
        }

        void Wait(Mutex mutex)
        {
            pthread_cond_wait(&_cond, mutex.Get());
        }

        void Signal()
        {
            pthread_cond_signal(&_cond);
        }

        void Broadcast()
        {
            pthread_cond_broadcast(&_cond);
        }

        ~Cond()
        {
            pthread_cond_destroy(&_cond);
        }
    private:
        pthread_cond_t _cond;
    };
}