/*************************************************************************
	> File Name: interrupt.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月07日 星期三 22时03分13秒
 ************************************************************************/

#include"../header.h"

void sigdeal1(int v)
{
    printf("111\n");
}

void sigdeal2(int v, siginfo_t * info, void * ptr)
{
    printf("222\n");
}

int main()
{
    struct sigaction act;
    struct sigaction oldact;
    act.sa_handler = NULL;
    act.sa_sigaction = sigdeal2;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    act.sa_restorer = NULL;

    sigaction(SIGUSR1, &act, &oldact);
    //while(1)
    //{
    //    sleep(1);
    //}
    getchar();

    printf("sig\n");
    return 0;
}
