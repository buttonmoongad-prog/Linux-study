#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include "mylog.hpp"
#include "InetAddr.hpp"
using namespace std;
using namespace LogModule;

const string default_path = "./dictionary.txt";
string sep = ": ";

class Dict
{
public:
    Dict(const string& path = default_path):_dick_path(path)
    {}

    void Load()
    {
        ifstream in(_dick_path);
        string line;
        while(getline(in,line))
        {
            auto it = line.find(sep);
            if(it == line.npos)
            {
                LOG(Level::Warning) << "该词条有问题";
                continue;
            }
            string english = line.substr(0, it);
            string chinese = line.substr(it + sep.size());
            if (english.empty() || chinese.empty())
            {
                LOG(Level::Warning) << "该词条有问题";
                continue;
            }
            _dict.insert(pair<string, string>(english, chinese));
            LOG(Level::Info) << "载入词条->" << english << ": " << chinese;
        }
        in.close();
    }

    string Translate(const string& word, InetAddr& client)
    {
        auto it = _dict.find(word);
        if(it == _dict.end())
        {
            return "None";
        }
        LOG(Level::Info) << "用户 [" << client.IP() << " " << client.Port() << "] 翻译:" << word << "->" << it->second; 
        return it->second;
    }

private:
    string _dick_path;
    unordered_map<string, string> _dict;
};