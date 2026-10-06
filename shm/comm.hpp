#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
using namespace std;
#define CREATE 1
#define USER 0

#define ERRO_EXIT(m)    \
do                      \
{                       \
    perror(m);          \
    exit(EXIT_FAILURE); \
} while(0)

class Shm
{
private:
    void CreaterHelper(size_t flg)
    {
        key_t key = ftok(".", 0x66);
        if (key < 0)
        {
            ERRO_EXIT("ftok");
        }
        _shmid = shmget(key, 4096, flg);
        if (_shmid < 0)
        {
            ERRO_EXIT("shmget");
        }
    }

    void Create()
    {
        CreaterHelper(IPC_CREAT | IPC_EXCL | 0666);
        _usetype = CREATE;
    }
    void Get()
    {
        CreaterHelper(IPC_CREAT);
    }

    void Attach()
    {
        _start_addr = shmat(_shmid, nullptr, 0);
        if ((long long)_start_addr < 0)
        {
            ERRO_EXIT("shmat");
        }
    }

    void Destroy()
    {
        if (_start_addr)
        {
            shmdt(_start_addr);
        }
    }
public:
    Shm(int usetype)
    :_shmid(-1), _start_addr(nullptr),_usetype(usetype)
    {
        if (_usetype == CREATE)
        {
            Create();
        }
        else
        {
            Get();
        }
        Attach();
    }

    void* VirtualAddress()
    {
        return _start_addr;
    }

    ~Shm()
    {
        Destroy();
        if (_usetype == CREATE)
        {
            shmctl(_shmid, IPC_RMID, nullptr);
        }
    }

private:
    int _shmid;
    void* _start_addr;
    int _usetype;
};