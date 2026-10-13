#include "thread.hpp"
using namespace PthreadMod;

int main()
{
    Thread t([](){
        while (true)
        {
            cout << "我是一个新线程" << endl;
            sleep(1);
        }
    });
    t.Detach();
    t.Start();

    // t.Detach();
    sleep(5);
    
    t.Stop();
    sleep(5);

    t.Join();

    return 0;
}