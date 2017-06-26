/*************************************************************************
	> File Name: acceptProve.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月17日 星期六 16时27分54秒
 ************************************************************************/

/*多进程调用accept函数提高运行效率*/
#include"../network.h"

//子进程处理函数
void handle(int server)
{
    while(1)//让5个进程不退出
    {
        int newfd = myAccept(server, NULL, NULL);//5个进程抢任务
        printf("%d, 0x%x\n", newfd, (int)getpid());
        close(newfd);
    }
}

int main()
{
    int server = myServer(9988, "0.0.0.0");

    int forkNumber = 5;
    int i;
    for(i=0; i<forkNumber; ++i)
    {
        pid_t pid = fork();//子进程创建
        if(pid == 0)
        {
            handle(server);
            return 0;
        }
    }

    //收尸
    for(i=0; i<forkNumber; ++i)
    {
        wait(NULL);
    }
    close(server);
    return 0;
}
