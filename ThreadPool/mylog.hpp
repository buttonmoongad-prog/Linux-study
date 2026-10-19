#ifndef __LOG__
#define __LOG__
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <time.h>
#include <unistd.h>
#include "Cond && Mutex.hpp"
using namespace std;
using namespace CondAndMutexModule;

namespace LogModule
{
    const string gsep = "\r\n";
    class LogStrategy
    {
    public:
        ~LogStrategy() = default;
        virtual void SyncLog(const string& message) = 0;
    };

    const string defaultpath = "./log/";
    const string defaultfile = "my.log";
    class FileLogStrategy : public LogStrategy
    {
    public:
        FileLogStrategy(const string& path = defaultpath, const string& file = defaultfile):_path(path), _file(file)
        {
            LockGraud lockgraude(_mutex);
            if (filesystem::exists(_path))
            {
                return;
            }
            try
            {
                filesystem::create_directories(_path);
            }
            catch (filesystem::filesystem_error e)
            {
                cerr << e.what() << gsep;
            }
        }
        void SyncLog(const string& message) override
        {
            LockGraud lockgraude(_mutex);
            const string filename = _path + (_path.back() == '/' ? "" : "/") + _file;
            ofstream out(filename, std::ios::app);
            if (!out)
            {
                return;
            }
            out << message << gsep;
            out.close();
        }

        ~FileLogStrategy()
        {

        }
    private:
        string _path;
        string _file;
        Mutex _mutex;
    };


    class ConsoleLogStrategy : public LogStrategy
    {
    public:
        ConsoleLogStrategy()
        {
        }
        void SyncLog(const string& message) override
        {
            LockGraud loggraud(_mutex);
            cerr << message << gsep;
        }
    private:
        Mutex _mutex;
    };

    enum class Level
    {
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };

    string GetLocalTime()
    {
        time_t tm = time(nullptr);
        struct tm cerr_tm;
        localtime_r(&tm, &cerr_tm);
        stringstream ss;
        ss << cerr_tm.tm_year + 1900<< '-'
            << cerr_tm.tm_mon + 1 << '-'
            << cerr_tm.tm_mday << ' '
            << cerr_tm.tm_hour << ':'
            << cerr_tm.tm_min << ':'
            << cerr_tm.tm_sec;
        return ss.str();
    }

    string LevelToString(const Level& level)
    {
        switch (level)
        {
        case Level::Debug:
            return "Debug";
        
        case Level::Info:
            return "Info";
        
        case Level::Warning:
            return "Warning";

        case Level::Error:
            return "Error";

        case Level::Fatal:
            return "Fatal";
        
        default:
            return "Unknown";
        }
    }

    class Logger
    {
    public:
        Logger()
        {
            EnableConsoleLogStrategy();
        }
        void EnableConsoleLogStrategy()
        {
            _fflush_strategy = make_unique<ConsoleLogStrategy>();
        }
        void EnableFileLogStrategy()
        {
            _fflush_strategy = make_unique<FileLogStrategy>();
        }
        class Log
        {
        public:
            Log(const Level& level, const string& filename, const int& linenum, Logger& logger)
                :_time(GetLocalTime())
                ,_level(level)
                ,_pid(getpid())
                ,_linenum(linenum)
                ,_filename(filename)
                ,_logger(logger)
            {
                stringstream ss;
                ss << '[' << _time << ']'
                    << '[' << LevelToString(_level) << ']'
                    << '[' << _pid << ']'
                    << '[' << _filename << ']'
                    << '[' << _linenum << ']';
                _logfrontmessage = ss.str();
            }

            template<class T>
            Log& operator<<(const T& message)
            {
                stringstream ss;
                ss << '-' << message;
                _logfrontmessage += ss.str();
                return *this;
            }

            ~Log()
            {
                if(_logger._fflush_strategy)
                {
                    _logger._fflush_strategy->SyncLog(_logfrontmessage);
                }

            }

        private:
            string _time;
            Level _level;
            pid_t _pid;
            int _linenum;
            string _filename;
            string _logfrontmessage;
            Logger& _logger;
        };
        Log operator()(Level level, string filename, int linenum)
        {
            return Log(level, filename, linenum, *this);
        }

    private:
        unique_ptr<LogStrategy> _fflush_strategy;
    };
    Logger logger;
    #define LOG(level) logger(level, __FILE__, __LINE__)
    #define Enable_Console_Log_Strategy() logger.EnableConsoleLogStrategy();
    #define Enable_File_Log_Strategy() logger.EnableFileLogStrategy();
}

#endif
