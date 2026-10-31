#include "UdpServer.hpp"
#include "thread.hpp"
using namespace PthreadMod;

int _sockfd = 0;
string ip;
uint16_t port = 0;
pthread_t id;

void Send()
{
    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    
    while (true)
    {
        string in;
        cout << "Please Enter:";
        getline(cin, in);
        int s = sendto(_sockfd, in.c_str(), in.size(), 0, (struct sockaddr*)&server, sizeof(server));
        if (s < 0)
        {
            LOG(Level::Error) << "send fail";
        }
        if (in == "QUIT")
        {
            pthread_cancel(id);
            break;
        }
    }
}

void Recv()
{
    while(true)
    {
        char buffer[1024];
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int n = recvfrom(_sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&peer, &len);
        if (n > 0)
        {
            buffer[n] = 0;
            cerr << buffer << endl;
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cerr << "Debug" << "./udpclient ip port" << endl;
        return 1;
    }

    ip = argv[1];
    port = stoi(argv[2]);

    _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (_sockfd < 0)
    {
        LOG(Level::Error) << "socket fail";
        return 2;
    }

    Thread sender(Send);
    Thread recver(Recv);

    sender.Start();
    recver.Start();

    id = recver.Id();

    sender.Join();
    recver.Join();

    return 0;
}