#include "UdpServer.hpp"
#include "Router.hpp"
#include <functional>
#include "ThreadPool.hpp"
using namespace ThreadPoolModule;
using task_t = function<void()>;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        LOG(Level::Debug) << "./udpserver port";
        return 1;
    }
    uint16_t port = stoi(argv[1]);

    Router r;

    auto tp = ThreadPool<task_t>::GetInstance();

    unique_ptr<UdpServer> server = make_unique<UdpServer>(port, [&r, &tp](int sockfd, const string& message, InetAddr& peer){
        task_t t = std::bind(&Router::MessageRouter, &r, sockfd, message, peer);
        tp->Equeue(t);
    });
    server->Init();
    server->Start(); 

    return 0;
}