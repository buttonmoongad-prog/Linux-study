#include <queue>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

template<class T>
class BlockQueue
{
public:
    BlockQueue(): _cap(5), _csleep_nums(0), _psleep_nums(0)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_full_cond, nullptr);
        pthread_cond_init(&_empty_cond, nullptr);
    }

    void Equeue(const T& in)
    {
        pthread_mutex_lock(&_mutex);
        if(isFull())
        {
            _csleep_nums++;
            pthread_cond_wait(&_full_cond, &_mutex);
            _csleep_nums--;
        }

        _q.push(in);
        if (_psleep_nums > 0)
            pthread_cond_signal(&_empty_cond);
        pthread_mutex_unlock(&_mutex);
    }

    T Pop()
    {
        pthread_mutex_lock(&_mutex);
        if (isEmpty())
        {
            _psleep_nums++;
            pthread_cond_wait(&_empty_cond, &_mutex);
            _psleep_nums--;
        }

        int data = _q.front();
        _q.pop();
        if (_csleep_nums > 0)
            pthread_cond_signal(&_full_cond);
        pthread_mutex_unlock(&_mutex);
        return data;
    }

    ~BlockQueue(){}

private:
    bool isFull() 
    {
        if(_q.size() == _cap) 
            return true;
        else
            return false;
    }
    bool isEmpty()
    {
        if (_q.size() == 0)
            return true;
        else
            return false;
    }
    queue<T> _q;
    const int _cap;
    int _csleep_nums;
    int _psleep_nums;
    pthread_mutex_t _mutex;
    pthread_cond_t _full_cond;
    pthread_cond_t _empty_cond;
};