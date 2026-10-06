#include "comm.hpp"

int main()
{
    Shm ser(CREATE);
    char* m = (char*)ser.VirtualAddress();
    int cnt = 10;
    while(true)
    {
        printf("%s\n", m);
        cnt--;
        if (cnt == 0)
            break;
        sleep(1);
    }
    return 0;
}