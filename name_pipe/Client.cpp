#include "comm.hpp"

int main()
{
    NamePipe pi(".", "fifo");
    pi.OpenWritePipe();
    pi.Write();

    return 0;
}