/*************************************************************************
	> File Name: forkexec.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月03日 星期六 20时18分24秒
 ************************************************************************/

#include "../header.h"

int main()
{
    pid_t pid = fork();

    //让子进程变成wf.out的执行程序
    if (pid == 0)
    {
        int fd = open("b.txt", O_WRONLY);

        execlp("./wf.out", "./wf.out", NULL);
        printf("error\n");
    }

    //父进程收尸体
    wait(NULL);
    printf("child clear\n");
}
