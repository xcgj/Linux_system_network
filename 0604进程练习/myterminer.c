/*************************************************************************
	> File Name: myterminer.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月04日 星期日 11时09分58秒
 ************************************************************************/

#include "../header.h"

/* 
 * 接收命令
 * 检查命令，并处理终端效果
 * 处理命令
 *  命令拆分到指针数组
 *  内置cd命令
 *      chdiar *getenv(const char *name);
 *  用户命令
 *      execvp
 * */

char g_cmd[4096] = {0};//接收命令字符串
char * cmdlist[1024] = {0};//拆分命令列表
int g_argcount = 0;//命令行参数个数

//获得命令
void getcmd()
{
    //打印shell提示符，path$    getcwd()
    //char *getcwd(char *buf, size_t size);    
    char curpath[1024] = {0};
    getcwd(curpath, sizeof(curpath));
    printf("%s$", curpath);

    //获得用户命令
    //char *fgets(char *s, int size, FILE *stream);
    fgets(g_cmd, sizeof(g_cmd), stdin);
    //处理\n
    g_cmd[strlen(g_cmd) - 1] = '\0';
}

void splitcmd()
{
    //按空格拆分 strtok
    //char *strtok(char *str, const char *delim);
    //The strtok() function breaks a string into a sequence of zero  or  more nonempty tokens.  On the first call to strtok() the string to be parsed hould be specified in str.  In each subsequent call that should  parse  the same string, str must be NULL. 
    char * arg = strtok(g_cmd, " \t");
    cmdlist[g_argcount++] = arg;

    while(arg)
    {
        arg = strtok(NULL, " \t");

        cmdlist[g_argcount] = arg;//地址是g_cmd字符串中的地址，全局的
        ++g_argcount;
    }

    /*
    int i = 0;
    for(; ; ++i)
    {
        if(cmdlist[i] == NULL)
        {
            break;
        }
        printf("%s  ", cmdlist[i]);
    }
    */
}

//处理命令
void dealcmd()
{
    //将命令拆分到数组
    splitcmd();

    //内置命令处理
    if (strcmp(cmdlist[0], "cd") == 0)
    {
        if (cmdlist[1] != NULL)
        {
            chdir(cmdlist[1]);//切换指定目录
        }
        else//用户主目录
        {
            chdir(getenv("HOME"));
        }
        return;
    }

    //普通命令处理
    pid_t pid = fork();
    if (pid == 0)
    {
        //子进程成为进程组组长
        setpgid(getpid(), getpid());

        execvp(cmdlist[0], cmdlist);
        printf("程序执行失败\n");

        return;
    }
    else
    {
        wait(NULL);
    }
    return;
}

int main()
{
    while(1)
    {
        getcmd();

        if (strlen(g_cmd) == 0)
        {
            continue;//空命令
        }

        dealcmd();
    }
}
