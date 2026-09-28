#include"pipe.hpp"

int main()
{
    ProcessPipe tpp(5);
    tpp.Create();
    int count = 10;
    while (count--)
    {
        tpp.Run();
        sleep(1);
    }

    tpp.Quit();
    return 0;
}