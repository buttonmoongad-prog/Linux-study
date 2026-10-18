#include "RingQueue.hpp"
#include <unistd.h>

void* consumer(void* args)
{
    RingQueue<int>* rq = static_cast<RingQueue<int>*>(args);

    while(true)
    {
        sleep(1);
        int n;
        rq->Pop(&n);
        cout << "消费了一个数据："<< n << endl << endl;;
    }

}

void* producter(void* args)
{
    RingQueue<int>* rq = static_cast<RingQueue<int>*>(args);
    int cnt = 1;
    while(true)
    {
        rq->Equeue(cnt);
        cout << "生产了一个数据："<< cnt++ << endl << endl;;
    }
}


int main()
{
    RingQueue<int> *rq = new RingQueue<int> ();
    
    pthread_t c[2], p[3];
    pthread_create(c, nullptr, consumer, rq);
    pthread_create(c + 1, nullptr, consumer, rq);
    pthread_create(p, nullptr, producter, rq);
    pthread_create(p + 1, nullptr, producter, rq);
    pthread_create(p + 2, nullptr, producter, rq);


    pthread_join(c[0],nullptr);
    pthread_join(c[1],nullptr);
    pthread_join(p[0],nullptr);
    pthread_join(p[1],nullptr);
    pthread_join(p[2],nullptr);


    return 0;
}