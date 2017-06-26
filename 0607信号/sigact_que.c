/*************************************************************************
	> File Name: sigact_que.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月07日 星期三 21时25分38秒
 ************************************************************************/

#include"../header.h"

void func1(int v)//信号处理函数
{

}

void func2(int v, siginfo_t * inf, void * ptr)//信号处理函数
{
    printf("int\n");
    //获取参数
    printf("%d\n", inf->si_int);
    printf("%s\n", (char*)ptr);
}

int main()
{
    //定义参数
    struct sigaction act;
    //act.sa_handler = func1;
    act.sa_handler = NULL;
    act.sa_sigaction = func2;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);//当正在处理信号函数时，SIGINT暂时屏蔽，直到信号函数处理结束，自动解除屏蔽
    act.sa_flags = SA_SIGINFO;//指定flag收取参数
    act.sa_restorer = NULL;

    struct sigaction oldact;

    //注册信号处理函数
    sigaction(SIGINT, &act, &oldact);

    //测试信号
    //kill(getpid(), SIGINT);
    union sigval val;
    val.sival_int = 10000;
    sigqueue(getpid(), SIGINT, val);
    sleep(5);

    return 0;
}
