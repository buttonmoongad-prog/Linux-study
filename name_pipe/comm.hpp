#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;
#define readpi 1
#define writepi 0

class NamePipe 
{
public:
    NamePipe(const string &path, const string &name) 
    : _path(path), _name(name), _fd(-1), method(writepi)
    {
        _filename = _path + "/" + _name;
    }

    bool MakePipe() 
    {
        int n = mkfifo(_filename.c_str(), 0666);
        if (n == 0) 
        {
            cout << "make pipe success" << endl;
            method = readpi;
            return true;
        } else 
        {
            cerr << "make pipe error" << endl;
            return false;
        }
    }

    void OpenReadPipe() 
    {
        _fd = open(_filename.c_str(), O_RDONLY);
        if (_fd < 0)
        {
            cerr << "open read fail" << endl;
            return;
        }
        cout << "open read success" << endl;
    }

    void OpenWritePipe() 
    {
        _fd = open(_filename.c_str(), O_WRONLY);
        if (_fd < 0) 
        {
            cerr << "open write fail" << endl;
            return;
        }
        cout << "open write success" << endl;
    }

    void Read() 
    {
        while (1) 
        {
            char buffer[1024];
            int n = read(_fd, buffer, sizeof(buffer) - 1);
            if (n > 0) 
            {
                buffer[n] = 0;
                cout << "Client Say:" << buffer << endl;
            } else if (n == 0) 
            {
                cout << "Quit" << endl;
                break;
            } 
            else
            {
                cerr << "read error" << endl;
                break;
            }
        }
    }

    void Write()
    {
        string message;
        int cnt = 1;
        pid_t id = getpid();
        while (1)
        {
            cout << "Please Enter:";
            getline(cin, message);
            message += (", message number:" + to_string(cnt++) + ",[" + to_string(id) + "]");
            write(_fd, message.c_str(), message.size());
        }
    }

    void Close()
    {
        if(_fd > 0)
        {
            close(_fd);
        }
    }

    ~NamePipe() 
    {
        if (method)
        {
            Close();
            int n = unlink(_filename.c_str());
            if (n == 0)
            {
                cout << "unlink pipe" << endl;
            }
            else
            {
                cerr << "unlink error" << endl;
            }
        }
        else
        {
            Close();
        }
    }

private:
    string _path;
    string _name;
    string _filename;
    int _fd;
    int method;
};