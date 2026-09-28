#include <climits>
#include <cstdlib>
#include<ctype.h>
#include<iostream>
#include<cstdio>
#include <memory>
#include<pthread.h>
#include <sched.h>
#include<stdlib.h>
#include<cstring>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

#define HOSTNAME "VM-0-17-ubuntu"
#define FORMAT "%s@%s:%s# "
#define COMMAND_SIZE 1024
#define MAXRACE 128

int exitcode = 0;

#define NONE_REDIR 0
#define INPUT_REDIR 1
#define OUTPUT_REDIR 2
#define APPEND_REDIR 3
std::string filename;
int redir = NONE_REDIR;

int g_argc = 0;
char* g_argv[MAXRACE];

char cwd[1020];
char cwdenv[1024];

char oldpwdenv[1024];

const char* GETUSERNAME()
{
    return getenv("USER");
}

const char* GETPWD()
{
    const char* pwd = getcwd(cwd, sizeof(cwd));
    if(pwd != NULL)
    {
        snprintf(cwdenv, sizeof(cwdenv), "PWD=%s", cwd);
        putenv(cwdenv);
    }
    return getenv("PWD");
}

const char* GETOLDPWD()
{
    const char* oldpwd = getenv("OLDPWD");
    return oldpwd;
}

void CHOLDPWD(const char* old)
{
    snprintf(oldpwdenv, sizeof(oldpwdenv), "OLDPWD=%s", old);
    putenv(oldpwdenv);
}

const char* GETHOME()
{
    return getenv("HOME");
}

void MakeCommandLine(char cmd_command[], int size)
{
    snprintf(cmd_command, size, FORMAT, GETUSERNAME(), HOSTNAME, GETPWD());
}

void PrintCommandLine()
{
    char prompt[COMMAND_SIZE];
    MakeCommandLine(prompt, sizeof(prompt));
    printf("%s", prompt);
    fflush(stdout);
}

bool GetCommandLine(char* out, int size)
{
    char* com = fgets(out, size, stdin);
    if (com == nullptr)
        return false;
    out[strlen(out) - 1] = 0;
    if (strlen(out) == 0)
        return false;
    return true;
}

bool CommandTable(char* cmd_prompt)
{
#define SEP " "
    g_argc = 0;
    g_argv[g_argc++] = strtok(cmd_prompt, SEP);
    while(g_argv[g_argc++] = strtok(nullptr, SEP));
    if(g_argv[g_argc] == nullptr)
        g_argc--;
    return g_argc > 0 ? true : false;
}

void CdHome()
{
    std::string home = GETHOME();
    chdir(home.c_str());
}

void Cd()
{
    if (g_argc == 1)
    {
        CdHome();
    }
    else
    {
        std::string ch = g_argv[1];
        if (ch == "-")
        {
            const char* oldpwd = getenv("PWD");
            const char* pwd = GETOLDPWD();
            chdir(pwd);
            CHOLDPWD(oldpwd);
        }
        else if(ch == "~")
        {
            CdHome();
        }
        else
        {
            std::string where = g_argv[1];
            chdir(where.c_str());
        }
    }
}

void Echo()
{
    if (g_argc == 2)
    {
        std::string opt = g_argv[1];
        if (opt == "$?")
        {
            std::cout << exitcode << std::endl;
            exitcode = 0;
        }
        
        else if(opt[0] == '$')
        {
            std::string envname = opt.substr(1);
            const char* envvalue = getenv(envname.c_str());
            if (envvalue)
                std::cout << envvalue << std::endl;
        }

        else
        {
            std::cout << opt << std::endl;
        }
    }
}

bool IsInBuiltCommand()
{
    std::string cmd = g_argv[0];
    if (cmd == "cd")
    {
        Cd();
        return true;
    }

    else if (cmd == "echo")
    {
        Echo();
        return true;
    }
    return false;
}

void TrimSpace(char cmd[], int& end)
{
    while (isspace(cmd[end]))
    {
        end++;
    }
}

void RedirCheck(char cmd[])
{
    redir = NONE_REDIR;
    filename.clear();
    int begin = 0;
    int end = strlen(cmd) - 1;
    while (end > begin)
    {
        // < >> >
        if (cmd[end] == '<')
        {
            cmd[end++] = 0;
            TrimSpace(cmd, end);
            redir = INPUT_REDIR;
            filename = cmd + end;
            break;
        }
        else if(cmd[end] == '>')
        {
            if (end - 1 >= 0 && cmd[end - 1] == '>')
            {
                cmd[end - 1] = 0;
                redir = APPEND_REDIR;
            }
            else
            {
                redir = OUTPUT_REDIR;
            }
            cmd[end++] = 0;    
            TrimSpace(cmd, end);
            filename = cmd + end;
            break;
        }
        else
        {
            end--;
        }
    }
}

int Execute()
{
    pid_t id = fork();
    if (id == 0)
    {
        if (redir == INPUT_REDIR)
        {
            int fd = open(filename.c_str(), O_RDONLY);
            if (fd < 0) exit(2);
            dup2(fd, 0);
        }
        else if (redir == OUTPUT_REDIR)
        {
            int fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0666);
            if (fd < 0) exit(2);
            dup2(fd, 1);
        }
        else if(redir == APPEND_REDIR)
        {
            int fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0666);
            if (fd < 0) exit(2);
            dup2(fd, 1);
        }
        execvp(g_argv[0], g_argv);
        exit(1);
    }
    int status = 0;
    pid_t rid = waitpid(id, &status, 0);
    if (rid > 0)
    {
        exitcode = WEXITSTATUS(status);
    }
    return 0;
}

int main()
{
    while(true)
    {
        //输出命令行提示符
        PrintCommandLine();

        //输入命令
        char Command[COMMAND_SIZE];
        if(!GetCommandLine(Command, COMMAND_SIZE))
        {
            continue;
        }

        RedirCheck(Command);

        //分析命令
        if(!CommandTable(Command))
            continue;
        
        //判断是否为内建命令
        if(IsInBuiltCommand())
        {
            continue;
        }

        //执行命令
        Execute();
    }

    return 0;
}
