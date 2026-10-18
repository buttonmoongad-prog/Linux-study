#pragma once
#include <iostream>
#include <semaphore.h>

namespace SemModule
{
    class Sem
    {
    public:
        Sem(unsigned int capacity = 1):_capacity(capacity)
        {
            sem_init(&_sem, 0, _capacity);
        }

        void P()
        {
            sem_wait(&_sem);
        }

        void V()
        {
            sem_post(&_sem);
        }

        ~Sem()
        {
            sem_destroy(&_sem);
        }
    private:
        sem_t _sem;
        int _capacity;
    };
}