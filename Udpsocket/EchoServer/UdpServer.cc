#include "UdpServer.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        LOG(Level::Debug) << "./udpserver port";
        return 1;
    }
    uint16_t port = stoi(argv[1]);
    unique_ptr<EchoServer> server = make_unique<EchoServer>(port);
    server->Init();
    server->Start(); 

    return 0;
}