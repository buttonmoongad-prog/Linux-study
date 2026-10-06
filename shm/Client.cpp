#include "comm.hpp"

int main()
{
    Shm shm(USER);
    char* m = (char*)shm.VirtualAddress();
    for (char c = 'A'; c <= 'Z'; c++)
    {
        m[c - 'A'] = c;
        sleep(1);
    }
    return 0;
}