
#include "../header.h"

void signal_deal(int v)
{
    printf("v = %d\n", v);
}

int main()
{
    //重写了SIGHUP处理函数，session 领导退出，不会影响其他进程退出
    //告诉内核，当SIGHUP信号发生时，调用signal_deal函数
    signal(SIGHUP, signal_deal);

    while(1)
    {
        sleep(1);
    }

    //getchar();//不能演示SIGHUP的原因是因为getchar和终端有关系
    return 0;
}
