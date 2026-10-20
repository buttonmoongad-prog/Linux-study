#include "ThreadPool.hpp"
#include "Task.hpp"
using namespace ThreadPoolModule;

int main()
{
    for (int i = 0; i < 5; i++)
    {
        ThreadPool<task_t>::GetInstance()->Equeue(DownLoad);
        sleep(1);
    }

    ThreadPool<task_t>::GetInstance()->Stop();
    // sleep(5);

    ThreadPool<task_t>::GetInstance()->Join();
    sleep(2);

    return 0;
}