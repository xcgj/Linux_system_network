/*************************************************************************
	> File Name: wait.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月03日 星期六 17时03分39秒
 ************************************************************************/

#include "../header.h"

int main()
{
    pid_t pid = fork();

    if (pid == 0)//子进程
    {
        //子进程结束
        return 233;
    }    
    else
    {
        /*
        //让子进程先挂
        sleep(1);
        //父进程不退出，没有回收PCB,所以子进程变成了僵尸
        getchar();
        */
    
        //wait函数用于收尸
        //由于一个父进程可能有很多子进程，而且可能有很多子进程都处于僵尸状态
        //所以它有一个返回值，表示收到的尸体是哪个子进程的
        //这也意味着一次wait只能收一个尸体
        //收尸的同时，也获得子进程的运行结果，运行结果会被放入status
        //如果不关心结果，可以用NULL代替status
        //wait是一个阻塞调用，如果没有尸体，会一直等尸体出现才会继续向下执行
        int status;
        pid_t pid = wait(&status);//将子进程改变的状态放入status，返回值是子进程的pid
        //pid_t pid = wait(NULL);

        //status有32位，其中只有8位用于保存子进程的返回值，其他位有其他用途
        if (WIFEXITED(status))
        {
            printf("子进程正常退出\n");
            printf("子进程的返回值是：%d\n", WEXITSTATUS(status));//程序正常退出才能调用WEXITSTATUS
        }
    }

    return 0;
}

