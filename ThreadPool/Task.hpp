#pragma once
#include <iostream>
#include <functional>
#include "mylog.hpp"
using namespace std;
using namespace LogModule;

using task_t = function<void()>;
void DownLoad()
{
    LOG(Level::Info) << "这是一个下载任务";
}