#pragma once
#include "Cond && Mutex.hpp"
#include "Sem.hpp"
#include <vector>

using namespace SemModule;
using namespace CondAndMutexModule;
using namespace std;

template<class T>
class RingQueue
{
public:
    RingQueue(unsigned int cap = 5)
        :_cap(cap)
        ,_rq(cap)
        ,_data_sem(0)
        ,_c_step(0)
        ,_blank_sem(cap)
        ,_p_step(0)
    {

    }

    void Equeue(const T& in)
    {
        _blank_sem.P();
        {
            LockGraud plock(_pmutex);
            _rq[_p_step] = in;
            _p_step++;
            _p_step %= _cap;
        }
        _data_sem.V();
    }

    void Pop(T* out)
    {
        _data_sem.P();
        {        
            LockGraud clock(_cmutex);
            *out = _rq[_c_step];
            _c_step++;
            _c_step %= _cap;
        }
        _blank_sem.V();
    }

    ~RingQueue()
    {

    }
private:
    int _cap;
    Mutex _cmutex;
    Mutex _pmutex;
    Sem _data_sem;
    int _c_step;
    Sem _blank_sem;
    int _p_step;
    vector<T> _rq;
};