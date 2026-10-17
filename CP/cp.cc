#include "BlockQueue.hpp"

void* consumer(void* args)
{
    BlockQueue<int>* bq = static_cast<BlockQueue<int>*>(args);

    while(true)
    {
        sleep(1);
        int n = bq->Pop();
        cout << "消费了一个数据："<< n << endl << endl;;
    }

}

void* producter(void* args)
{
    BlockQueue<int>* bq = static_cast<BlockQueue<int>*>(args);
    int cnt = 1;
    while(true)
    {
        bq->Equeue(cnt);
        cout << "生产了一个数据："<< cnt++ << endl << endl;;
    }
}


int main()
{
    BlockQueue<int> *bq = new BlockQueue<int> ();
    
    pthread_t c, p;
    pthread_create(&c, nullptr, consumer, bq);
    pthread_create(&p, nullptr, producter, bq);

    pthread_join(c,nullptr);
    pthread_join(p,nullptr);


    return 0;
}