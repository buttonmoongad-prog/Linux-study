#include "mylog.hpp"
using namespace LogModule;

int main()
{
    Enable_Console_Log_Strategy();
    LOG(Level::Debug) << "hello world" << " 3.14";
    LOG(Level::Debug) << "hello world" << " 3.14";

    Enable_File_Log_Strategy();
    LOG(Level::Debug) << "hello world" << " 3.14";
    LOG(Level::Debug) << "hello world" << " 3.14";
    return 0;
}