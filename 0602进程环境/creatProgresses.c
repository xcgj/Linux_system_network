/*************************************************************************
	> File Name: creatProgresses.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月02日 星期五 16时35分08秒
 ************************************************************************/

#include <unistd.h>
#include <stdio.h>

void forks(int n)
{
    int i = 0;
    for(; i < n; ++i)
    {
        pid_t pid = fork();
        //如果是子进程就不要再创建子类进程了，直接跳出循环
        //如果是父进程继续循环产生子进程
        if(pid == 0)
        {
            break;
        }
    }
}

int main()
{
    forks(10);

    printf("xcgj\n");
    return 0;
}
