/*************************************************************************
	> File Name: fork.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月02日 星期五 15时06分15秒
 ************************************************************************/
#include "../header.h"
#include<stdio.h>
#include<unistd.h>

int g_var = 0;
int main()
{
    printf("11111\n");
    pid_t pid = fork();//创建了一个新的进程
    //fork()之前的代码不会被执行，但是会影响fork()进程，fork()进程含有之前的代码
    //从这句代码开始，程序会有2个进程，意味着代码中的变量被搞成了两份
    //一份给子进程，一份给父进程

    //子进程和父进程的pid变量分别执行了以下判断
    if(pid == 0)//子进程fork函数的返回值，执行这个判断的是子进程
    {
        printf("child's fork() return pid = %d\n", pid);
        printf("child's ppid = %d\n", getppid());//获得父进程的pid
        g_var = 100;//子进程改变全局变量的值不会影响父进程
        //因为这是两个进程了，各自有自己的全局变量
    }

    else  if(pid >0)//子进程的pid，但执行这个判断的是父进程
    {
        //当fork返回值大于0时，说明该进程是父进程
        //此时，返回值就是子进程的pid
        printf("parent's fork() return pid(child's pid) = %d\n", pid);
        printf("parent's pid = %d\n", getpid());
        //父进程无法调用函数读取子进程的pid，因为子进程可能有多个
        printf("g_var = %d\n", g_var);
    }

    else
    {
        perror("fork");
    }

    printf("xcgj\n");

    getchar();
    return 0;
}
