/*************************************************************************
  > File Name: shell.c
  > Author: 陈琳耀 || C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月05日 星期一 18时45分45秒
 ************************************************************************/

#include "../header.h"

void printtip()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("%s << myshell >> ", cwd); 
}

#if 0 
//执行简单的命令
void dealcmd(char * cmd)
{
    //创建子进程执行命令
    pid_t pid = fork();

    if (pid == 0)
    {
        execlp(cmd, cmd, NULL);
        exit(0);//为了执行错误命令时也能退出
    }

    wait(NULL);
}
#endif

#if 0
//多个命令
void dealcmd(char * cmd)
{
    //数组接收命令行命令，execvp执行
    char * comds[1024];
    int i = 0;
    char * arg = strtok(cmd, " \t");
    comds[i++] = arg;

    while(arg != NULL)
    {
        arg = strtok(NULL, " \t");
        comds[i++] = arg;
    }

    //子进程执行
    pid_t pid = fork();
    if (pid == 0)
    {
        execvp(comds[0], comds);
        exit(0);
    }

    wait(NULL);
}
#endif

//增加重定向功能
void dealcmd(char * cmd)
{
    //检查是不是追加重定向
    int isAppend = 0;//标识符
    if (strstr(cmd, ">>"))
    {
        isAppend = 1;
    }

    //先查找重定向命令
    //查找 >> >前的命令
    char * cmd1 = strtok(cmd, ">");
    //查找 >> >后面的文件名
    char * cmd2 = strtok(NULL, ">");

    //清空文件名内包含的空格
    char * filename = strtok(cmd2, " \t");
    //printf("***%s***\n", cmd1);
    //printf("***%s***\n", filename);

    //处理命令
    //数组接收命令行命令，execvp执行
    char * comds[1024];
    int i = 0;
    char * arg = strtok(cmd1, " \t");
    comds[i++] = arg;

    while(arg != NULL)
    {
        arg = strtok(NULL, " \t");
        comds[i++] = arg;
    }

    //子进程执行
    pid_t pid = fork();
    if (pid == 0)
    {
        if (filename)//filename不为NULL才重定向
        {
            int flag = O_WRONLY|O_CREAT|O_TRUNC;
            if (isAppend)
            {
                flag = O_WRONLY|O_CREAT|O_APPEND;
            }
            int fd = open(filename, flag, 0777);
            //文件描述符替换
            dup2(fd, 1);
            close(fd);
        }
        execvp(comds[0], comds);
        exit(0);
    }

    wait(NULL);
}

//cd
int dealnestcmd(char* cmd)
{
    //拷贝辅助变量，不对源命令动手脚
    char * cmdtmp = strdup(cmd);

    int cd = 1;//返回值标志
    //切割字符串看看是不是cd命令
    char * arg1 = strtok(cmdtmp, " \t");
    if (strcmp(arg1, "cd")==0)
    {
        cd = 0;//说明是cd命令

        char * arg2 = strtok(NULL, " \t");
        if (arg2 == NULL)//home目录判断
        {
            arg2 = getenv("HOME");
        }
        chdir(arg2);
    }
    
    return cd;
}

int main()
{
    while(1)
    {
        // 打印提示
        printtip();

        // 获得命令
        char cmd[1024];
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strlen(cmd) - 1] = 0;//去\n 

        //判断是不是内置命令  cd为例
        if (dealnestcmd(cmd))//如果是cd，返回0，不是返回1，默认不是
        {
            // 不是cd的话，就解析命令
            dealcmd(cmd);
        }
    }
        return 0;
}



