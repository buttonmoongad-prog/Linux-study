#pragma once
#include<iostream>
#include<unistd.h>
#include<vector>
#include<ctime>
using namespace std;

typedef void (*Task)();

void Printlog()
{
    cout << "Printlog" << endl;
}

void Download()
{
    cout << "Download" << endl;
}

void Update()
{
    cout << "Update" << endl;
}

class TaskManage
{
public:
    TaskManage()
    {
        srand(time(nullptr));
    }

    void Register(Task t)
    {
        _task.push_back(t);
    }

    int Code()
    {
        int code = rand() % _task.size();
        return code;
    }

    void Execute(int code)
    {
        if (code >= 0 && code < _task.size())
        {
            _task[code]();
        }
    }

    ~TaskManage(){}


private:
    vector<Task> _task;
};