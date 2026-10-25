#include "UdpServer.hpp"
#include "Dict.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        LOG(Level::Debug) << "./udpserver port";
        return 1;
    }
    uint16_t port = stoi(argv[1]);
    Dict dict;
    dict.Load();
    unique_ptr<EchoServer> server = make_unique<EchoServer>(port, [&dict](const string& word, InetAddr& cli)->string{
        return dict.Translate(word, cli);
    });
    server->Init();
    server->Start(); 

    return 0;
}