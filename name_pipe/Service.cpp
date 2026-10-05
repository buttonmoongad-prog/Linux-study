#include "comm.hpp"

int main()
{
    NamePipe pi(".", "fifo");
    if(pi.MakePipe())
    {
        pi.OpenReadPipe();
        pi.Read();
    }

    return 0;
}