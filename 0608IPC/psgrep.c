/*************************************************************************
	> File Name: psgrep.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月08日 星期四 16时26分51秒
 ************************************************************************/

#include"../header.h"

int main()
{
    //ps aux
    //grep a.out
    int fd[2];
    int ret = pipe(fd);

    pid_t pid = fork();
    if (pid == 0)
    {
        //close(fd[0]);//关闭 读
        //重定向
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execlp("ps", "ps", "aux", NULL);
        return 0;
    }

    pid_t pid2 = fork();
    if (pid2 == 0)
    {
//        close(fd[1]);
        //重定向
        dup2(fd[0], 0);
        close(fd[1]);
        close(fd[0]);
        execlp("grep", "grep", "a.out", NULL);
        return 0;
    }

    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);

    return 0;
}
