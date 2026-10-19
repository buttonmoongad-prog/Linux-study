#include "ThreadPool.hpp"
#include "Task.hpp"
using namespace ThreadPoolModule;

int main()
{
    ThreadPool<task_t> threadpool(5);
    
    threadpool.Start();
    sleep(5);

    for (int i = 0; i < 5; i++)
    {
        threadpool.Equeue(DownLoad);
        sleep(1);
    }

    threadpool.Stop();
    // sleep(5);

    threadpool.Join();
    sleep(2);

    return 0;
}